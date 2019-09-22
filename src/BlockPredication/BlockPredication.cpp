#include "BlockPredication/BlockPredication.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace tas {

bool BlockPredication::run() {
  setPathStateForEachCondBranch();
  F->print(errs());
  return true;
}

void BlockPredication::savePathState(BranchInst * BI, AllocaInst * AI) {
  IRBuilder<> Builder(F->getContext());
  auto PathIDMap = PPA.getBlockToPathIDMapRef();
  auto Pred = BI->getCondition();
  Builder.SetInsertPoint(BI);
  auto TruePathID = PathIDMap.FindAndConstruct(BI->getSuccessor(0)).getSecond();
  auto FalsePathID = PathIDMap.FindAndConstruct(BI->getSuccessor(1)).getSecond();
  auto PathIdVal = Builder.CreateSelect(Pred, Builder.getInt32(TruePathID), Builder.getInt32(FalsePathID));
  Builder.CreateStore(PathIdVal, AI);
}

void BlockPredication::setPathStateForEachCondBranch() {
  // Find conditional branch instructions, for each branch
  // instruction insert "select" instruction which sets
  // pathid variable corresponding to branch targets.
  // Branch instruction is a terminator instruction, hence
  // look for each terminator instruction in basic block.
  IRBuilder<> Builder(F->getContext());
  Builder.SetInsertPoint(&F->getEntryBlock().front());
  auto PathIdAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  for (auto & BB : *F) {
    if (auto * BI = dyn_cast<BranchInst>(BB.getTerminator())) {
      if (!BI->isConditional()) continue;
      savePathState(BI, PathIdAlloca);
    }
  }
}

}
