#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/Debug.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <string>

#include "Util.h"

using namespace llvm;

namespace tas {

std::string VarAnnotationStr = "llvm.var.annotation";

/// This function returns a map of Function and annotated string in the module.
void getAnnotatedFunctionList(Module * M, DenseMap<Function *, StringRef> & FnList) {
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

void detectExpensivePointerVariables(Function * F, SmallVectorImpl<Value *> & ExpensivePointers) {
  auto varAnnotationIntrinsic = Function::lookupIntrinsicID("llvm.var.annotation");
  // XXX Checking only entry basic block for annotated variables.
  for (auto & I : F->front()) {
    if (auto * CI = dyn_cast<CallInst>(&I)) {

      // Check whether var.annotation or not
      auto * Callee = CI->getCalledFunction();
      if (!Callee->isIntrinsic() || Callee->getIntrinsicID() != varAnnotationIntrinsic) continue;

      // Get annotation string literal
      auto StringTag = cast<ConstantDataArray>(
          cast<GlobalVariable>(
            CI->getOperand(1)->stripPointerCasts())->getOperand(0))->getAsCString();

      if (StringTag.compare("expensive") == 0) {
        ExpensivePointers.push_back(cast<BitCastInst>(CI->getArgOperand(0))->getOperand(0));
      }
    }
  }
}

void detectBatchingParameters(Function * F, SmallPtrSet<Value *, 4> & BatchParameters) {
  auto varAnnotationIntrinsic = Function::lookupIntrinsicID("llvm.var.annotation");
  // XXX Checking only entry basic block for annotated variables.
  for (auto & I : F->front()) {
    if (auto * CI = dyn_cast<CallInst>(&I)) {

      // Check whether var.annotation or not
      auto * Callee = CI->getCalledFunction();
      if (!Callee->isIntrinsic() || Callee->getIntrinsicID() != varAnnotationIntrinsic) continue;

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

// Insert Prefetch instruction BEFORE the instruction I.
void insertLLVMPrefetchIntrinsic(Function * F, Instruction * I, Value * PtrVal) {
  // Set prefetch instruction insertion point.
  IRBuilder<> Builder(F->getContext());
  Builder.SetInsertPoint(I->getPrevNode());

  // Cast pointer value to i8* type
  auto CastI = Builder.CreateBitCast(PtrVal, Builder.getInt8PtrTy(), "TAS-inst1");

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
    // We should advance iterator first, because changing user modifies use list hence
    // invalidates the iterator.
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
  if (auto * CI = dyn_cast<ConstantInt>(GEP->getOperand(GEP->getNumIndices()))) {
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

}
