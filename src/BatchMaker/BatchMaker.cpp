#include "BatchMaker.h"
#include "BatchProcess/BatchProcess.h"
#include "Common/ForLoop.h"
#include "Common/Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <deque>
#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-maker"

namespace tas {

/// Old Function Prototype:
///    Ret Fn(Type1 A, Type2 B BATCH_ARG, Type3 C BATCH_ARG);
/// BatchForm Fn Prototype:
///    void Fn_batch(Type1 A, Type2 * B, Type3 * C, int16_t TAS_BATCHSIZE, Ret * TAS_RETURNS);
Function * BatchMaker::createBatchedFormFnPrototype() {
  errs() << "Function = " << NonBatchFunc->getName() << "\n";

  // Name of the argument to be batched is prefixed with string "batch_arg_i", where
  // i goes from [1, ArgsToBatch.size()]
  std::string prefix { "batch_arg_" }; int i = 0;
  unsigned BatchParamIndex = 0;
  for (auto & Arg : NonBatchFunc->args()) {
    if (ArgsToBatch.find(&Arg) != ArgsToBatch.end()) {
      BatchArgTypes.push_back(PointerType::get(Arg.getType(), 0));
      BatchArgNames.push_back(prefix + std::to_string(i++));
      BatchParamIndices.push_back(BatchParamIndex++);
    } else {
      // Add argument unmodified
      BatchArgTypes.push_back(Arg.getType());
      BatchArgNames.push_back(Arg.getName());
      BatchParamIndex++;
    }
  }

  // Adding Parameter representing actual batch size during run time
  BatchArgTypes.push_back(Type::getInt16Ty(NonBatchFunc->getContext()));
  BatchArgNames.push_back(std::string("TAS_BATCHSIZE"));

  auto RetType = NonBatchFunc->getReturnType();
  // Add pointer as output parameter, where return value is stored.
  BatchArgTypes.push_back(PointerType::get(RetType, 0));
  BatchArgNames.push_back(std::string("TAS_RETURNS"));

  // Create Function prototype
  FunctionType *BatchFuncType = FunctionType::get(RetType, BatchArgTypes, false);
  auto F = Function::Create(BatchFuncType, GlobalValue::ExternalLinkage,
                                        NonBatchFunc->getName() + "_batch", NonBatchFunc->getParent());
  return F;
}

void BatchMaker::setArgumentNamesInBatchFunc() {
  // Set argument names.
  auto NewArgIt = BatchFunc->arg_begin();
  for (int i = 0; i < BatchFunc->arg_size() - 1; ++i) {
    NewArgIt->setName(BatchArgNames[i]);
    if (i == BatchParamIndices.front()) {
      BatchArgs.push_back(&*NewArgIt);
      BatchParamIndices.pop_front();
    }
    ++NewArgIt;
  }

  NewArgIt->setName(BatchArgNames[BatchFunc->arg_size() - 1]);
  RetArg = &*NewArgIt;
}

bool BatchMaker::run() {

  detectBatchingParameters(NonBatchFunc, ArgsToBatch);
  detectExpensivePointerVariables(NonBatchFunc, PrefetchVars);
  BatchFunc = createBatchedFormFnPrototype();
  setArgumentNamesInBatchFunc();

  updateBasicBlocksInBatchFunc();
  DominatorTree DT (*BatchFunc);
  LoopInfo LI (DT);
  auto BP = BatchProcess(BatchFunc, &LI, &DT);
  bool changed = BP.run();
  BatchFunc->print(errs());

  return true;
}

void BatchMaker::updateBasicBlocksInBatchFunc() {

  ValueToValueMapTy VMap;
  auto NewA = BatchFunc->arg_begin();
  for (const Argument & A : NonBatchFunc->args()) {
    VMap[&A] = &*NewA++;
  }

  SmallVector<ReturnInst*, 8> Returns;  // Ignore returns cloned
  CloneFunctionInto(BatchFunc, NonBatchFunc, VMap, NonBatchFunc->getSubprogram() != nullptr, Returns);

  auto EntryBB = &BatchFunc->getEntryBlock();
  IRBuilder<> Builder(EntryBB);
  Builder.SetInsertPoint(EntryBB, EntryBB->begin());

  // For each argument, replace all uses.
  SmallPtrSet<Value *, 4> BatchedAllocas;
  SmallVector<Value *, 4> BatchGEPs;
  for (auto & A : BatchArgs) {
    auto APtr = Builder.CreateAlloca(A->getType());
    BatchedAllocas.insert(APtr);
    //Builder.CreateStore(A, APtr);
    StoreInst * StoreI = nullptr;
    for (auto * U : A->users()) {
      if (auto * I = dyn_cast<StoreInst>(U)) {
        StoreI = I;
      }
    }

    // Store argument in alloca variable.
    AllocaInst * OldAlloca = nullptr;
    if (StoreI) {
      if ((OldAlloca = dyn_cast<AllocaInst>(StoreI->getPointerOperand())))
        StoreI->setOperand(1, APtr);
    } else {
      continue;
    }

    // Replace old alloca variable uses with new alloca variable.
    // Old variable contained a single pointer, hence access would be single load op.
    // New alloca variable contains double pointer.
    // Hence dereference would be 3 op : load -> getelementptr -> load 
    auto NumUses = OldAlloca->getNumUses();
    while (NumUses > 0) {
      User * U = OldAlloca->user_back();
      Builder.SetInsertPoint(cast<Instruction>(U));
      auto DerefAPtr = Builder.CreateLoad(APtr);
      auto ElemPtr = Builder.CreateGEP(DerefAPtr, Builder.getInt64(0) /*FIXME add index var*/);
      BatchGEPs.push_back(ElemPtr);
      U->replaceUsesOfWith(OldAlloca, ElemPtr);
      NumUses--;
    }
  }

  // Store Ret parameter in alloca.
  auto RetAlloca = Builder.CreateAlloca(RetArg->getType());
  Builder.CreateStore(RetArg, RetAlloca);

  // Find first use of batch alloca and split there.
  // New basic block going to be part of batch processing loop.
  BasicBlock * BatchCodeStartBlock = nullptr;
  for (inst_iterator I = inst_begin(BatchFunc), E = inst_end(BatchFunc); I != E; ++I) {
    if (isa<LoadInst>(*I) && isa<GetElementPtrInst>(I->getNextNode()) &&
        isa<LoadInst>(I->getNextNode()->getNextNode())) {

      bool Found = false;
      for (const auto * V : I->operand_values()) {
        if (BatchedAllocas.find(V) != BatchedAllocas.end()) {
          BatchCodeStartBlock = I->getParent()->splitBasicBlock(BasicBlock::iterator(*I), "BatchBlock_begin");
          Found = true;
          break;
        }
      }
      if (Found) break;
    }
  }


  // Make loop body to have a single backedge.
  // For that we need to tie all exiting edges and point it to single block.
  SmallVector<BasicBlock *, 4> TerminatingBB;
  for (inst_iterator I = inst_begin(BatchFunc), E = inst_end(BatchFunc); I != E; ++I) {
    if (ReturnInst * Return = dyn_cast<ReturnInst>(&*I)) {
      TerminatingBB.push_back(Return->getParent());
    }
  }

  auto EndBlock = BasicBlock::Create(BatchFunc->getContext(), "EndBlock", BatchFunc);
  ReturnInst::Create(BatchFunc->getContext(), Constant::getNullValue(NonBatchFunc->getReturnType()),
                      EndBlock);

  // XXX Can be improved, if there is one terminating block, then that itself be knot block.
  auto KnotBlock = BasicBlock::Create(BatchFunc->getContext(), "Knotblock", BatchFunc);
  BranchInst::Create(EndBlock, KnotBlock);

  SmallVector<Value *, 4> RetVals;
  for (auto & BB : TerminatingBB) {
    RetVals.push_back(BB->getTerminator()->getOperand(0));
    ReplaceInstWithInst(BB->getTerminator(), BranchInst::Create(KnotBlock));
  }

  // Use Dominator tree to decide which alloca index need to be replaced.
  auto DT = DominatorTree(*BatchFunc);

  auto * TripCount = BatchFunc->getValueSymbolTable()->lookup("TAS_BATCHSIZE");
  assert (TripCount && "Trip count argument must be given");

  int i = 1;
  if (BatchCodeStartBlock) {
    auto TL0 = TASForLoop(BatchFunc->getContext(), &BatchFunc->getEntryBlock(), EndBlock,
        "tas.loop." + std::to_string(i), BatchFunc, TripCount);
    TL0.setLoopBody(BatchCodeStartBlock, KnotBlock);
    // Set offset as loop index variable in BatchGEPs.
    auto IndexVar = cast<Instruction>(TL0.getIndexVariable64Bit());
    for (auto & V : BatchGEPs) {
      auto GI = cast<Instruction>(V);
      if (!DT.dominates(GI, BatchCodeStartBlock))
        GI->setOperand(GI->getNumOperands() - 1, IndexVar);
    }

    // Store return value in a temporary variable.
    auto RetVal = RetVals.begin();
    for (auto & BB : TerminatingBB) {
      Builder.SetInsertPoint(BB->getTerminator());
      auto ptr = Builder.CreateLoad(RetAlloca);
      auto offset = Builder.CreateGEP(ptr, IndexVar);
      Builder.CreateStore(*RetVal++, offset);
    }
  }
}

} // tas namespace
