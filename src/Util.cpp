#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/Debug.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include "Util.h"

using namespace llvm;

namespace tas {

// FIXME Use intrinsic call name for detection instead of enum value
Intrinsic::ID VarAnnotationId = static_cast<Intrinsic::ID>(177);

void setAnnotationInFunctionObject(Module * M) {
  auto AnnotationList = M->getNamedGlobal("llvm.global.annotations");
  if (!AnnotationList) return;

  auto CA = cast<ConstantArray>(AnnotationList->getOperand(0));
  for (unsigned int i = 0; i < CA->getNumOperands(); ++i) {
    auto CAStruct =cast<ConstantStruct>(CA->getOperand(i));
    auto CAFunc = dyn_cast<Function>(CAStruct->getOperand(0)->getOperand(0));
    if (!CAFunc) continue;
    auto CAAnnotation = cast<ConstantDataArray>(
        cast<GlobalVariable>(CAStruct->getOperand(1)->getOperand(0))->getOperand(0));
    CAFunc->addFnAttr(CAAnnotation->getAsCString());
  }
}

void detectVarAnnotation(Function * F, SmallVectorImpl<Instruction *> & EI) {
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    // First argument to llvm.var.annotation intrinsic is the value to be annotated.
    // Address is of type i8*. BitCast instruction is used before intrinsic call
    // to convert variable pointer type to i8*. Operand to BitCast instruction
    // is the Value we are interested in.
    auto * CI = dyn_cast<CallInst>(&*I);
    if(!CI) continue;
      
    if (CI->getCalledFunction()->getIntrinsicID() != VarAnnotationId)
      continue;

    //LLVM_DEBUG(dbgs() << "Use of annotated ptr in load instruction\n");
    auto * ExpensivePtr = (cast<BitCastInst>(CI->getOperand(0)))->getOperand(0);

    // If annotated variable is of type pointer, then we are interested in load with
    // address contained in pointer variable. In LLVM, allocas are used for local variable,
    // accessing allocas are done using load/store op. Such load/store is not
    // expensive hence we don't track it.
    for (User * U : ExpensivePtr->users()) {
      auto PtrInst = dyn_cast<LoadInst>(U);
      if (!PtrInst) continue;
      for (User *PtrUse : PtrInst->users())
        if (auto * ExpLoad = dyn_cast<LoadInst>(PtrUse))
          EI.push_back(ExpLoad);
    }
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
}
