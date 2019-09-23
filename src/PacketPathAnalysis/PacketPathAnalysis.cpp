#include "PacketPathAnalysis.h"

#include <llvm/Analysis/PostDominators.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Debug.h>

#include <algorithm>
#include <cassert>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "tas"

namespace tas {

void PacketPathAnalysis::recalculate() {
  IntermediateBBPathIdMap.clear();
  PathExitingBlocksToPathIDMap.clear();
  PathIDToBLockList.clear();
  computePathTrace();
}

void PacketPathAnalysis::visitPredecessor(BasicBlock * BB, unsigned PathID) {
  for (auto Pred : predecessors(BB)) {
    if (IntermediateBBPathIdMap[Pred].find(PathID) != IntermediateBBPathIdMap[Pred].end())
      continue;

    IntermediateBBPathIdMap[Pred].insert(PathID);
    PathIDToBLockList[PathID].push_back(Pred);
    if (find(TotalOrder.begin(), TotalOrder.end(), Pred) == TotalOrder.end())
      PathOrder.push_back(Pred);
    visitPredecessor(Pred, PathID);
  }
}

void PacketPathAnalysis::computePathTrace() {
  PostDominatorTree PDT(*F);
  DominatorTree DT(*F);
  auto & EntryBlock = F->getEntryBlock();
  // Check all the return blocks.
  SmallVector<BasicBlock *, 4> ReturnBlocks;
  for (BasicBlock & BB : *F)
    if (isa<ReturnInst>(BB.getTerminator()))
      ReturnBlocks.push_back(&BB);

  // Assume Function has single exit block.
  // TODO Else, make function single exit block.
  // Use unifysingleexitnode pass.
  assert (ReturnBlocks.size() == 1);
  ReturnBlock = ReturnBlocks.front();

  int i = 1;
  for (auto * BB : predecessors(ReturnBlock))
    PathExitingBlocksToPathIDMap.insert(make_pair(BB, i++));

  LLVM_DEBUG(errs() << "Number of Paths = " << PathExitingBlocksToPathIDMap.size() << "\n");

  // Mark each predecessor of path exiting block with path id.
  for (auto & KV : PathExitingBlocksToPathIDMap) {
    auto * EB = KV.getFirst();
    auto PathID = KV.getSecond(); 
    PathOrder.push_back(EB);
    visitPredecessor(EB, PathID);
    TotalOrder.insert(TotalOrder.end(), PathOrder.rbegin(), PathOrder.rend());
    PathOrder.clear();
  }

  // Remove duplicate blocks.
  prepareFinalMap();

 // dumpDebugDataToConsole();
}

void PacketPathAnalysis::prepareFinalMap() {
  // Prepare final Basic block to Path ID map.
  // If block already has one id, that means it is part of some path.
  // In that case, remove all ids and assign 0.
  // 0 means this basic block will be executed for all kind of packet paths.
  // XXX This meaning is not true for all cases. Suppose there are 3 paths,
  // them some basic block can be part of 2 paths. I am not sure how to handle it
  // correctly at this moment.
  for (auto & KV : IntermediateBBPathIdMap) {
    auto & V = KV.getSecond();
    if (V.size() > 1) {
      MiddleBlockToPathIdMap.insert(make_pair(KV.getFirst(), 0));
      BlockToPathIdMap.insert(make_pair(KV.getFirst(), 0));
    } else {
      assert (V.size() == 1);
      MiddleBlockToPathIdMap.insert(make_pair(KV.getFirst(), *V.begin()));
      BlockToPathIdMap.insert(make_pair(KV.getFirst(), *V.begin()));
    }
  }
  for (auto & KV : PathExitingBlocksToPathIDMap) {
    BlockToPathIdMap.insert(make_pair(KV.getFirst(), KV.getSecond()));
  }
}

void PacketPathAnalysis::dumpDebugDataToConsole() {
  errs() << "Basic block path information\n";
  errs() << "Path Exiting Blocks\n";
  for (auto & E : PathExitingBlocksToPathIDMap) {
    auto & BB = E.getFirst();
    BB->printAsOperand(errs());
    errs() << " : ";
    auto & ID = E.getSecond();
    errs() << to_string(ID) << "  ";
    errs() << "\n";
  }
  errs() << "Block to Path memebers map\n";
  for (auto & E : IntermediateBBPathIdMap) {
    auto & BB = E.getFirst();
    BB->printAsOperand(errs());
    errs() << " : ";
    auto & IDs = E.getSecond();
    for (auto & ID : IDs) {
      errs() << to_string(ID) << "  ";
    }
    errs() << "\n";
  }

  errs() << "Path information\n";
  for (auto & E : PathIDToBLockList) {
    auto ID = E.getFirst();
    errs() << to_string(ID) << " : ";
    for (auto & BB : E.getSecond()) {
      BB->printAsOperand(errs());
      errs() << "  ";
    }
    errs() << "\n";
  }

  errs() << "Total Order Size = " << TotalOrder.size() << "\n";
  errs() << "Total Execution Order\n";
  for (auto & BB : TotalOrder) {
    BB->printAsOperand(errs());
    errs() << " --> ";
  }
  errs() << "\n";
}

}
