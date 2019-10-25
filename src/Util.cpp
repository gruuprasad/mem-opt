#include <llvm/ADT/SmallVector.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <llvm/Analysis/DominanceFrontier.h>
#include <llvm/Analysis/PostDominators.h>
#include <llvm/Analysis/RegionInfo.h>


#include <memory.h>
#include <string>

#include "Util.h"

using namespace llvm;
using namespace std;

namespace tas {

// Information to decode annotation fields:
// Custom annotations added to the source code is stored as global value
// in LLVM IR under the identifer "llvm.global.annotations".
// This function adds the annotation identifier to the attribute list of
// Function object of in-memory IR. This facilitates applying function level
// transformation to the function of interest to us.
//
// Annotation list structure in IR: stored as array of struct
// ConstantArray : [Size x struct]
// struct def: [Function  ptr, GlobalVariable ptr, GlobalVariable ptr, i32]
// @llvm.global.annotations = [N x {i8*, i8*, i8*, i32}]
// N - number of global annotations in a module
// Struct members details:
// i8* - Function pointer
// i8* - Pointer to annotation string
// i8* - Pointer to file name string
// i32 - line number of annotation in source file

/// This function returns a map of Function and annotated string in the module.

static std::string VarAnnotationStr = "llvm.var.annotation";

void getAnnotatedFnList(Module * M, map<Function *, string> & FnList) {
  auto AnnotationList = M->getNamedGlobal("llvm.global.annotations");
  if (!AnnotationList) return;

  auto CA = cast<ConstantArray>(AnnotationList->getOperand(0));
  for (unsigned int i = 0; i < CA->getNumOperands(); ++i) {
    auto CAStruct = cast<ConstantStruct>(CA->getOperand(i));
    auto CAFunc = dyn_cast<Function>(CAStruct->getOperand(0)->getOperand(0));
    if (!CAFunc) continue;
    auto CAAnnotation = cast<ConstantDataArray>(
        cast<GlobalVariable>(CAStruct->getOperand(1)->getOperand(0))->getOperand(0));
    FnList[CAFunc] = CAAnnotation->getAsCString();
  }
}

SmallVector<Value *, 4> detectExpPtrVars(Function * F) {
  SmallVector<Value *, 4> ExpensivePointers;
  auto varAnnotationIntrinsic = Function::lookupIntrinsicID("llvm.var.annotation");
  // XXX Checking only entry basic block for annotated variables.
  for (auto & I : F->front()) {
    auto * CI = dyn_cast<CallInst>(&I);
    if (CI == nullptr) continue;

    // Check whether var.annotation or not
    auto * Callee = CI->getCalledFunction();
    if (!Callee->isIntrinsic() ||
        Callee->getIntrinsicID() != varAnnotationIntrinsic) continue;

    // Get annotation string literal
    auto StringTag = cast<ConstantDataArray>(
        cast<GlobalVariable>(
          CI->getOperand(1)->stripPointerCasts())->getOperand(0))->getAsCString();

    if (StringTag.compare("expensive") == 0) {
      ExpensivePointers.push_back(
                           cast<BitCastInst>(CI->getArgOperand(0))->getOperand(0));
    }
  }

  return ExpensivePointers;
}

const LoadInst * findEarliestPointerDerefInstruction(Value * V) {
  const LoadInst * LU = nullptr;
  auto matchLoadPattern = [&] (const auto * U)
  {
    if (llvm::isa<LoadInst>(U)) {
      auto NextI = cast<LoadInst>(U)->getNextNode();
      if (isa<llvm::GetElementPtrInst>(NextI) && NextI->getOperand(0) == U) {
        LU = cast<LoadInst>(U);
      } else if (isa<LoadInst>(NextI) && NextI->getOperand(0) == U) {
        LU = cast<LoadInst>(U);
      }
    }
  };

  for_each(V->users(), matchLoadPattern);
  return LU;
}

SmallVector<LoadInst *, 4> detectExpPtrUses(SmallVectorImpl<Value *> & AnnotatedVars) {
  SmallVector<LoadInst *, 4> VarUsePoints;
  
  for_each(AnnotatedVars, [&]
      (const auto & Var) { 
        auto FU = findEarliestPointerDerefInstruction(Var);
        errs() << *FU << "\n";
        if (!FU) return;
        VarUsePoints.push_back(const_cast<LoadInst *>(FU));
      });
  return VarUsePoints;
}

Instruction * findBatchBeginMarkerInstruction(Function * F) {
  auto varAnnotIntrinsic = Function::lookupIntrinsicID("llvm.var.annotation");
  // XXX Checking only entry basic block for annotated variables.
  for (auto I = inst_begin(*F), E = inst_end(*F); I != E; ++I) {
    auto * CI = dyn_cast<CallInst>(&*I);
    if (CI == nullptr) continue;

    // Check whether var.annotation or not
    auto * Callee = CI->getCalledFunction();
    if (!Callee->isIntrinsic() ||
        Callee->getIntrinsicID() != varAnnotIntrinsic) continue;

    // Get annotation string literal
    auto StringTag = cast<ConstantDataArray>(
        cast<GlobalVariable>(
          CI->getOperand(1)->stripPointerCasts())->getOperand(0))->getAsCString();

    if (StringTag.compare("batch_begin") == 0) {
      return CI;
    }
  }
  return nullptr;
}

void detectBatchParameters(Function * F, SmallPtrSet<Value *, 4> & BatchParameters) {
  auto varAnnotationIntrinsic = Function::lookupIntrinsicID("llvm.var.annotation");
  // XXX Checking only entry basic block for annotated variables.
  for (auto & I : F->front()) {
    auto * CI = dyn_cast<CallInst>(&I);
    if (CI == nullptr) continue;

    // Check whether var.annotation or not
    auto * Callee = CI->getCalledFunction();
    if (!Callee->isIntrinsic() ||
        Callee->getIntrinsicID() != varAnnotationIntrinsic) continue;

    // Get annotation string literal
    auto StringTag = cast<ConstantDataArray>(
        cast<GlobalVariable>(
          CI->getOperand(1)->stripPointerCasts())->getOperand(0))->getAsCString();

    if (StringTag.compare("batch_arg") == 0) {
      auto AllocaVar = cast<BitCastInst>(CI->getArgOperand(0))->getOperand(0);
      for (auto & I : F->front()) {
        if (auto * Store = dyn_cast<StoreInst>(&I)) {
          if (Store->getPointerOperand() == AllocaVar)
            BatchParameters.insert(Store->getValueOperand());
        }
      }
    }
  }
}

void setAnnotationInFunctionObject(Module * M) {
  auto AnnotationList = M->getNamedGlobal("llvm.global.annotations");
  if (!AnnotationList) return;

  auto CA = cast<ConstantArray>(AnnotationList->getOperand(0));
  for (unsigned int i = 0; i < CA->getNumOperands(); ++i) {
    auto CAStruct = cast<ConstantStruct>(CA->getOperand(i));
    auto CAFunc = dyn_cast<Function>(CAStruct->getOperand(0)->getOperand(0));
    if (!CAFunc) continue;
    auto CAAnnotation = cast<ConstantDataArray>(
        cast<GlobalVariable>(CAStruct->getOperand(1)->getOperand(0))->getOperand(0));
    CAFunc->addFnAttr(CAAnnotation->getAsCString());
  }
}

void cloneLoopBasicBlocks(Function * F, Loop * L, ValueToValueMapTy & VMap) {
  SmallVector<BasicBlock *, 16> ClonedBlocks;
  for (auto * BB : L->blocks()) {
    auto * ClonedBlock = CloneBasicBlock(BB, VMap, "_clone");
    VMap[BB] = ClonedBlock;
    ClonedBlocks.push_back(ClonedBlock);

    auto ExitBlock = L->getExitBlock();
    assert(ExitBlock != nullptr && "Support only loop with single exit edge");

    ClonedBlock->insertInto(F, ExitBlock);
  }
  remapInstructionsInBlocks(ClonedBlocks, VMap);

  // Set cloned loop as successor of old loop
  auto LoopExitingBlock = L->getExitingBlock();
  assert(LoopExitingBlock != nullptr && "Support only loop with single exit edge");
  auto LoopTerminator = LoopExitingBlock->getTerminator();
  assert(LoopTerminator != nullptr && "Loop Basicblock is not well formed");
  LoopTerminator->setSuccessor(1, ClonedBlocks.front());
}

void insertLLVMPrefetchIntrinsic(Function * F, Instruction * PtrAllocaUse) {
  IRBuilder<> Builder(PtrAllocaUse);
  auto Ptr = Builder.CreateLoad(PtrAllocaUse->getOperand(0), "prefetch_load");
  auto CastI = Builder.CreateBitCast(Ptr, Builder.getInt8PtrTy(), "prefetch1");

  // Add llvm prefetch intrinsic call.
  Type *I32 = Type::getInt32Ty(F->getContext());
  Value *PrefetchFunc = Intrinsic::getDeclaration(F->getParent(), Intrinsic::prefetch);
  Builder.CreateCall(
      PrefetchFunc,
      {CastI, // Pointer Value
      ConstantInt::get(I32, 0), // read (0) or write (1)
      ConstantInt::get(I32, 3), // no_locality (0) to extreme temporal locality (3)
      ConstantInt::get(I32, 1)} // data (1) or instruction (0)
      );
}

/// Replace old value with new value within basic block.
void replaceUsesWithinBB(Value * From, Value * To, BasicBlock * BB) {
  auto UI = From->use_begin();
  while (UI != From->use_end()) {
    auto * Usr = dyn_cast<Instruction>(UI->getUser());
    if (Usr && Usr->getParent() != BB) {
      ++UI;
      continue;
    }
    // We should advance iterator first, because changing user
    // modifies use list hence invalidates the iterator.
    auto * U = &*UI++;
    U->set(To);
  }
}

/// Recursively compute the size of the struct
unsigned getTypeSizeInBits(Type * Ty) {
  if (!Ty->isAggregateType())
    return Ty->getPrimitiveSizeInBits();

  unsigned Total = 0;
  for (auto & subT : Ty->subtypes()) {
    Total += getTypeSizeInBits(subT);
  }
  return Total;
}



Value * createArray(Function * F, Type * Ty, unsigned size) {
  // Allocate temporary array
  IRBuilder<> Builder (F->getContext());
  auto BB = F->begin();
  auto TermI = (*BB).getTerminator();
  Builder.SetInsertPoint(TermI);
  return Builder.CreateAlloca(ArrayType::get(Ty, size));
}

template <typename InstType>
Instruction * findValueFirstUseInInstruction(Value * V) {
  InstType * FirstUse = nullptr;
  for (auto * U : V->users()) {
    if (auto * I = dyn_cast<InstType>(U)) {
      FirstUse = I;
    }
  }
  return FirstUse;
}

unsigned getGEPIndex(const GetElementPtrInst * GEP) {
  unsigned FieldIdx = 0;
  if (auto * CI = dyn_cast<ConstantInt>(
                   GEP->getOperand(GEP->getNumIndices()))) {
    FieldIdx = CI->getZExtValue();
  } else {
    assert (0 && "Value has to be constant expression!");
  }
  return FieldIdx;
}


StoreInst * findFirstUseInStoreInst(Value * V) {
  // User list is inverted, first user is at the end of the list.
  // Loop below finds first user which is StoreInst.
  StoreInst * SI = nullptr;
  for (auto * U : V->users()) {
    if (auto * I = dyn_cast<StoreInst>(U)) {
      SI = I;
    }
  }
  return SI;
}

void setSuccessor(BasicBlock * BB, BasicBlock * SuccBB, unsigned Idx) {
  auto TermI = BB->getTerminator();
  TermI->setSuccessor(Idx, SuccBB);
}

void cloneBasicBlocksInto(Function * From, Function * To) {
  // Store the mapping from old Value to new Value.
  ValueToValueMapTy VMap;
  auto ToArg = To->arg_begin();
  for (const Argument & FromArg : From->args()) {
    VMap[&FromArg] = &*ToArg++;
  }

  SmallVector<ReturnInst*, 8> Returns;  // Ignore returns cloned
  CloneFunctionInto(To, From, VMap, From->getSubprogram() != nullptr, Returns);
}

void getReturnBlocks(Function * F, SmallVectorImpl<BasicBlock *> & Returns) {
  for (BasicBlock & BB : *F)
    if (isa<ReturnInst>(BB.getTerminator()))
      Returns.push_back(&BB);
}

void getReturnInstList(Function * F, SmallVectorImpl<ReturnInst *> & Result) {
  // Check all the return blocks.
  for (BasicBlock & BB : *F)
    if (isa<ReturnInst>(BB.getTerminator()))
      Result.push_back((cast<ReturnInst>(BB.getTerminator())));
}

string writeToBitCodeFile(Module & M) {
  // Write to bitcode file.
  std::error_code EC;
  auto Idx = M.getSourceFileName().find_last_of(".");
  auto OutFile = M.getSourceFileName().substr(0, Idx) + string(".bc");
  raw_fd_ostream OS(OutFile, EC, llvm::sys::fs::F_None);
  WriteBitcodeToFile(M, OS);
  OS.flush();
  return OutFile;
}

string writeToAsmFile(Module & M) {
  // Write to .ll file.
  std::error_code EC;
  auto Idx = M.getSourceFileName().find_last_of(".");
  auto OutFile = M.getSourceFileName().substr(0, Idx) + string(".ll");
  raw_fd_ostream OS(OutFile, EC, llvm::sys::fs::F_None);
  M.print(OS, nullptr);
  OS.flush();
  return OutFile;
}

Value * addIncrementIndexOp(AllocaInst * IdxPtr, Instruction * InsertBefore) {
  IRBuilder<> Builder(InsertBefore);
  auto LoadVal = Builder.CreateLoad(IdxPtr);
  auto Incr = Builder.CreateAdd(LoadVal, Builder.getInt32(1));
  Builder.CreateStore(Incr, IdxPtr);
  return Incr;
}

void printRegeionInfo(Function * F) {
  auto DT = DominatorTree(*F);
  auto PDT = PostDominatorTree(*F);
  auto DF = DominanceFrontier();
  DF.analyze(DT);
  RegionInfo RI;
  RI.recalculate(*F, &DT, &PDT, &DF);

  errs() << "\n" << F->getName() << "\n";
  auto * R = RI.getTopLevelRegion();
  errs() << "=========RegionInfo========\n";
  int i = 0;
  for (auto & E : *R) {
    errs() << "Region " << to_string(i++) << ":\n";
    // Exiting blocks
    SmallVector<BasicBlock *, 4> ExitingBlocks;
    E->getExitingBlocks(ExitingBlocks);
    errs() << "Exiting blocks: ";
    for (auto & EB : ExitingBlocks) {
      EB->printAsOperand(errs());
      errs() << "  ";
    }
    errs() << "\n";
    ExitingBlocks.clear();
  }
}

pair<BasicBlock *, BasicBlock *> unifyFunctionExitNodes(Function &F) {
  vector<BasicBlock*> ReturningBlocks;
  vector<BasicBlock*> UnreachableBlocks;
  for (BasicBlock &I : F)
    if (isa<ReturnInst>(I.getTerminator()))
      ReturningBlocks.push_back(&I);
    else if (isa<UnreachableInst>(I.getTerminator()))
      UnreachableBlocks.push_back(&I);

  // Then unreachable blocks.
  BasicBlock * UnreachableBlock = nullptr;
  if (UnreachableBlocks.empty()) {
    UnreachableBlock = nullptr;
  } else if (UnreachableBlocks.size() == 1) {
    UnreachableBlock = UnreachableBlocks.front();
  } else {
    UnreachableBlock = BasicBlock::Create(F.getContext(),
                                          "UnifiedUnreachableBlock", &F);
    new UnreachableInst(F.getContext(), UnreachableBlock);

    for (BasicBlock *BB : UnreachableBlocks) {
      BB->getInstList().pop_back();  // Remove the unreachable inst.
      BranchInst::Create(UnreachableBlock, BB);
    }
  }

  // Now handle return blocks.
  if (ReturningBlocks.empty()) {
    return make_pair(nullptr, UnreachableBlock);            // No blocks return
  } else if (ReturningBlocks.size() == 1) {
    return make_pair(ReturningBlocks.front(), UnreachableBlock);
  }

  // Otherwise, we need to insert a new basic block into the function, add a PHI
  // nodes (if the function returns values), and convert all of the return
  // instructions into unconditional branches.
  //
  BasicBlock *NewRetBlock = BasicBlock::Create(F.getContext(),
                                               "UnifiedReturnBlock", &F);

  PHINode *PN = nullptr;
  if (F.getReturnType()->isVoidTy()) {
    ReturnInst::Create(F.getContext(), nullptr, NewRetBlock);
  } else {
    // If the function doesn't return void... add a PHI node to the block...
    PN = PHINode::Create(F.getReturnType(), ReturningBlocks.size(),
                         "UnifiedRetVal");
    NewRetBlock->getInstList().push_back(PN);
    ReturnInst::Create(F.getContext(), PN, NewRetBlock);
  }

  // Loop over all of the blocks, replacing the return instruction with an
  // unconditional branch.
  //
  for (BasicBlock *BB : ReturningBlocks) {
    // Add an incoming element to the PHI node for every return instruction that
    // is merging into this new block...
    if (PN)
      PN->addIncoming(BB->getTerminator()->getOperand(0), BB);

    BB->getInstList().pop_back();  // Remove the return insn
    BranchInst::Create(NewRetBlock, BB);
  }

  return make_pair(NewRetBlock, UnreachableBlock);
}

AllocaInst * getLoopIndexVar(Loop * L) {
  AllocaInst * Index = nullptr;
  auto LatchBB = L->getLoopLatch();
  assert(LatchBB && "Latch block can't be null!");

  // Find index increment instruction.
  // XXX Assume latch block contains only one add instruction, verify it.
  auto It = find_if(*LatchBB,
          [&] (const Instruction & I) {
          return (isa<BinaryOperator>(I) &&
                  I.getOpcode() == Instruction::Add &&
                  cast<ConstantInt>(I.getOperand(1))->equalsInt(1)) ? true : false; });

  Index = cast<AllocaInst>(cast<LoadInst>(It->getOperand(0))->getOperand(0));
  return Index;
}

} // namespace tas
