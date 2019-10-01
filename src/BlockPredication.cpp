#include "BlockPredication.h"
#include "Util.h"

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

void BlockPredication::linearizeControlFlow() {
  ReturnBlock = unifyFunctionExitNodes(*F);

  Builder.SetInsertPoint(&EntryBlock->front());
  MaskIDAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  auto MaskIDMap = PPA.getBlockPathCondition();

  // Set path state value according to branch target blocks.
  for (auto & BB : *F) {
    if (ReturnBlock == &BB) continue;

    if (auto * BI = dyn_cast<BranchInst>(BB.getTerminator())) {
      if (BI->isUnconditional()) continue;
      setPathIDCondition(BI, MaskIDMap);
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
    auto It = MaskIDMap.find(BB);
    if (It == MaskIDMap.end()) continue; // Could be Return Block

    auto MaskID = It->getSecond();
    auto PB = insertPredicateBlock(BB, MaskID);
    PredicateBlocks.push_back(PB);
  }

  // Set control flow: PredicateBlock[i]---[False]--> PredicateBlock[i+1]
  setPredicateBlocksFalseEdges();
}

void BlockPredication::setPathIDCondition(BranchInst * BI,
                                          BlockToIntMapType & MaskIDMap) {
  // Note: False dest index is 0, True Dest index is 1, counter intuitive.
  auto TruePathIt = MaskIDMap.find(BI->getSuccessor(1));
  auto FalsePathIt = MaskIDMap.find(BI->getSuccessor(0));

  // Sanity check
  assert (BI->getSuccessor(0) != nullptr && BI->getSuccessor(1) != nullptr);
  assert (TruePathIt != MaskIDMap.end() && FalsePathIt != MaskIDMap.end());

  Builder.SetInsertPoint(BI);
  auto PathIdVal = Builder.CreateSelect(BI->getCondition(),
                                        Builder.getInt32(TruePathIt->getSecond()),
                                        Builder.getInt32(FalsePathIt->getSecond()));
  Builder.CreateStore(PathIdVal, MaskIDAlloca);
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

BasicBlock * BlockPredication::insertPredicateBlock(BasicBlock * ActionBB,
                                                    unsigned PathID) {
  auto PB = BasicBlock::Create(F->getContext(),
            string("predicate_") + std::to_string(PathID), F, ActionBB);
  ActionBB->replaceAllUsesWith(PB);

  Builder.SetInsertPoint(PB);
  auto PathIDVal = Builder.CreateLoad(MaskIDAlloca);
  auto Pred = Builder.CreateICmp(CmpInst::ICMP_EQ, PathIDVal,
                                 Builder.getInt32(PathID));
  Builder.CreateCondBr(Pred, ActionBB, ActionBB);
  return PB;
}

void BlockPredication::setPredicateBlocksFalseEdges() {
  for (auto i = 0; i < PredicateBlocks.size() - 1; ++i) {
    setSuccessor(PredicateBlocks[i], PredicateBlocks[i+1]);
  }
  setSuccessor(PredicateBlocks.back(), ReturnBlock, 0);
}

}
