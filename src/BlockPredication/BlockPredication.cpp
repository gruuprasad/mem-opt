#include "BlockPredication/BlockPredication.h"
#include "Common/Util.h"

#include <llvm/ADT/PostOrderIterator.h>
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

void BlockPredication::setPathIDCondition(BranchInst * BI) {
  Builder.SetInsertPoint(BI);
  auto PathIDMap = PPA.getBlockToPathIDMapRef();
  assert (BI->getSuccessor(0) != nullptr);
  assert (BI->getSuccessor(1) != nullptr);
  auto TruePathIt = PathIDMap.find(BI->getSuccessor(0));
  auto FalsePathIt = PathIDMap.find(BI->getSuccessor(1));

  if (TruePathIt == PathIDMap.end() || FalsePathIt == PathIDMap.end())
    return;

  auto PathIdVal = Builder.CreateSelect(BI->getCondition(),
                                        Builder.getInt32(TruePathIt->getSecond()),
                                        Builder.getInt32(FalsePathIt->getSecond()));
  Builder.CreateStore(PathIdVal, PathIdAlloca);
}

void BlockPredication::setActionBlocksSuccessors() {
  for (int i = 0; i < ActionBlocks.size() - 1; ++i) {
    auto BI = cast<BranchInst>(ActionBlocks[i]->getTerminator());
    if (BI->isUnconditional())
      setSuccessor(ActionBlocks[i], ActionBlocks[i+1]);

    Builder.SetInsertPoint(BI);
    Builder.CreateBr(ActionBlocks[i+1]);
    BI->eraseFromParent();
  }
}

BasicBlock * BlockPredication::insertPredicateBlock(BasicBlock * ActionBB, unsigned PathID) {
  auto PB = BasicBlock::Create(F->getContext(),
            string("predicate_") + std::to_string(PathID), F, ActionBB);
  ActionBB->replaceAllUsesWith(PB);
  Builder.SetInsertPoint(PB);
  auto PathIDVal = Builder.CreateLoad(PathIdAlloca);
  auto Pred = Builder.CreateICmp(CmpInst::ICMP_EQ, PathIDVal, Builder.getInt32(PathID));
  Builder.CreateCondBr(Pred, ActionBB, ActionBB);
  return PB;
}

void BlockPredication::setPredicateBlocksFalseEdges() {
  for (auto i = 0; i < PredicateBlocks.size() - 1; ++i) {
    setSuccessor(PredicateBlocks[i], PredicateBlocks[i+1]);
  }
  setSuccessor(PredicateBlocks.back(), PPA.getReturnBlock(), 1);
}

void BlockPredication::linearizeControlFlow() {
  Builder.SetInsertPoint(&EntryBlock->front());
  PathIdAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  auto PathIDMap = PPA.getBlockToPathIDMapRef();

  // Set path state value according to branch target blocks.
  for (auto & BB : *F) {
    if (PPA.getReturnBlock() == &BB) continue;

    if (auto * BI = dyn_cast<BranchInst>(BB.getTerminator())) {
      if (BI->isUnconditional()) continue;
      setPathIDCondition(BI);
    }
  }

  // Get the Total execution order.
  ReversePostOrderTraversal<Function*> RPOT(F);
  for (auto B = RPOT.begin(); B != RPOT.end(); ++B) {
    ActionBlocks.push_back(*B);
  }

  // Set control flow to form linear execution.
  setActionBlocksSuccessors();

  // Entry block is executed for all paths.
  ActionBlocks.pop_front();

  // For each Action block add a Predicate block.
  // control flow: PredicateBlock --True--> ActionBlock.
  for (auto & BB : ActionBlocks) {
    auto It = PathIDMap.find(BB);
    if (It == PathIDMap.end()) continue; // Could be Return Block
    auto PathID = It->getSecond();
    auto PB = insertPredicateBlock(BB, PathID);
    PredicateBlocks.push_back(PB);
  }

  // Set control flow: PredicateBlock[i]---False--> PredicateBlock[i+1]
  setPredicateBlocksFalseEdges();
}

}
