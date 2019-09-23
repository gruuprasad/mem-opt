#include "BlockPredication/BlockPredication.h"
#include "Common/Util.h"

#include <llvm/IR/Instructions.h>

#include <algorithm>
#include <sstream>
#include <string>

using namespace llvm;
using namespace std;

namespace tas {

bool BlockPredication::run() {
  linearizeControlFlow();
  return true;
}

BasicBlock * BlockPredication::insertPredicateBlock(BlockIDPairType ActionBB, BasicBlock * SuccBB) {
  auto PB = BasicBlock::Create(F->getContext(),
            string("predicate_") + std::to_string(ActionBB.second), F, ActionBB.first);
  Builder.SetInsertPoint(PB);
  auto PathIDVal = Builder.CreateLoad(PathIdAlloca);
  auto Pred = Builder.CreateICmp(CmpInst::ICMP_EQ, PathIDVal, Builder.getInt32(ActionBB.second));
  Builder.CreateCondBr(Pred, ActionBB.first, SuccBB);
  return PB;
}

BasicBlock * BlockPredication::predicateIfElseBlock(BlockIDPairType IfActionBB, BlockIDPairType ElseActionBB) {
  assert (ElseActionBB.first->getUniqueSuccessor() != nullptr);
  auto ElsePred = insertPredicateBlock(ElseActionBB, ElseActionBB.first->getUniqueSuccessor());
  auto IfPred = insertPredicateBlock(IfActionBB, ElsePred);
  setSuccessor(IfActionBB.first, ElsePred);
  return IfPred;
}

void BlockPredication::flattenConditionBranchPaths(BranchInst * BI, AllocaInst * AI) {
  Builder.SetInsertPoint(BI);
  auto PathIDMap = PPA.getBlockToPathIDMapRef();
  auto TruePath = PathIDMap.FindAndConstruct(BI->getSuccessor(0));
  auto FalsePath = PathIDMap.FindAndConstruct(BI->getSuccessor(1));

  auto PathIdVal = Builder.CreateSelect(BI->getCondition(),
                                        Builder.getInt32(TruePath.getSecond()),
                                        Builder.getInt32(FalsePath.getSecond()));
  Builder.CreateStore(PathIdVal, AI);
  auto IfPred = predicateIfElseBlock(make_pair(TruePath.first, TruePath.second),
                       make_pair(FalsePath.first, FalsePath.second));
  Builder.SetInsertPoint(BI);
  Builder.CreateBr(IfPred);
  BI->eraseFromParent();
}

void BlockPredication::linearizeControlFlow() {
  // Find conditional branch instructions, for each branch
  // instruction insert "select" instruction which sets
  // pathid variable corresponding to branch targets.
  // Branch instruction is a terminator instruction, hence
  // look for each terminator instruction in basic block.
  Builder.SetInsertPoint(&F->getEntryBlock().front());
  PathIdAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  auto PathIDMap = PPA.getBlockToPathIDMapRef();

  for (auto KV : PathIDMap) {
    auto BB = KV.getFirst();

    if (PPA.getReturnBlock() == BB) continue;

    if (auto * BI = dyn_cast<BranchInst>(BB->getTerminator())) {
      if (BI->isUnconditional()) continue;
      flattenConditionBranchPaths(BI, PathIdAlloca);
    }
  }
}

}
