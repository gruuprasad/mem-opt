#include "BlockPredication.h"
#include "Util.h"

#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/Instructions.h>

#include <algorithm>
#include <sstream>
#include <string>
#include <tuple>

using namespace llvm;
using namespace std;

namespace tas {

bool BlockPredication::run() {
  linearizeControlFlow();
  return true;
}

void BlockPredication::linearizeControlFlow() {
  BasicBlock * UnreachableBlock;
  tie(ReturnBlock, UnreachableBlock) = unifyFunctionExitNodes(*F);

  Builder.SetInsertPoint(&EntryBlock->front());
  MaskIDAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  auto MaskIDMap = PPA.getBlockPathCondition();

  // Set path state value according to branch target blocks.
  for (auto & BB : *F) {
    if (ReturnBlock == &BB) continue;

    if (auto * BI = dyn_cast<BranchInst>(BB.getTerminator())) {
      setPathIDCondition(BI, MaskIDMap);
    }
  }

  // Get the Total execution order.
  ReversePostOrderTraversal<Function*> RPOT(F);
  for (auto B = RPOT.begin(); B != RPOT.end(); ++B) {
    // Leave unreachable block for now.
    if (*B == UnreachableBlock) continue;
    ActionBlocks.push_back(*B);
  }

  // Entry block is executed for all paths.
  ActionBlocks.pop_front();

  // For each Action block add a Predicate block.
  insertPredicateBlocks();
    
  setBlocksSuccessors(PredicateBlocks);

  setPredBlockSuccessors(PredicateBlocks, ActionBlocks);
}

void BlockPredication::setPathIDCondition(BranchInst * BI,
                                          BlockToIntMapType & MaskIDMap) {
  Builder.SetInsertPoint(BI);
  if (BI->isUnconditional()) {
    auto TargetIt = MaskIDMap.find(BI->getSuccessor(0));
    Builder.CreateStore(Builder.getInt32(TargetIt->getSecond()), MaskIDAlloca);
    return;
  }

  auto TruePathIt = MaskIDMap.find(BI->getSuccessor(0));
  auto FalsePathIt = MaskIDMap.find(BI->getSuccessor(1));

  // Sanity check
  assert (BI->getSuccessor(0) != nullptr && BI->getSuccessor(1) != nullptr);
  assert (TruePathIt != MaskIDMap.end() && FalsePathIt != MaskIDMap.end());

  auto PathIdVal = Builder.CreateSelect(BI->getCondition(),
                                        Builder.getInt32(TruePathIt->getSecond()),
                                        Builder.getInt32(FalsePathIt->getSecond()));
  Builder.CreateStore(PathIdVal, MaskIDAlloca);
}

void BlockPredication::setBlocksSuccessors(deque<BasicBlock *> & PredicateBlocks) {
  BranchInst * BI = cast<BranchInst>(F->getEntryBlock().getTerminator());
  BranchInst::Create(PredicateBlocks.front(), BI);
  BI->eraseFromParent();

  for (int i = 0; i < ActionBlocks.size() - 1; ++i) {
    BI = cast<BranchInst>(ActionBlocks[i]->getTerminator());
    BranchInst::Create(PredicateBlocks[i+1], BI);
    BI->eraseFromParent();
  }
}

void BlockPredication::insertPredicateBlocks() {
  for (int i = 0; i < ActionBlocks.size(); ++i) {
    auto PB = BasicBlock::Create(F->getContext(),
        string("predicate_") + std::to_string(i), F, ActionBlocks[i]);
    ActionBlocks[i]->replaceAllUsesWith(PB);
    PredicateBlocks.push_back(PB);
  }
}

// control flow: PredicateBlock --True--> ActionBlock.
// control flow: PredicateBlock[i]---[False]--> PredicateBlock[i+1]
void BlockPredication::setPredBlockSuccessors(deque<BasicBlock *> & PredBlocks,
                                              deque<BasicBlock *> & ActionBlocks) {
  auto MaskIDMap = PPA.getBlockPathCondition();
  for (auto i = 0; i < PredBlocks.size() - 1; ++i) {
    Builder.SetInsertPoint(PredBlocks[i]);
    auto It = MaskIDMap.find(ActionBlocks[i]);
    auto MaskID = It->getSecond();
    auto PathIDVal = Builder.CreateLoad(MaskIDAlloca);
    auto Pred = Builder.CreateICmp(CmpInst::ICMP_EQ, PathIDVal,
                                   Builder.getInt32(MaskID));
    Builder.CreateCondBr(Pred, ActionBlocks[i], PredBlocks[i+1]);
  }

  Builder.SetInsertPoint(PredBlocks.back());
  Builder.CreateBr(ReturnBlock);
}

}
