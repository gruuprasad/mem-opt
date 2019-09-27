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
  BlockToPathSet.clear();
  ExitingBlockPathIDMap.clear();
  PathIDToBLockList.clear();
  computePathTraces();
}

void PacketPathAnalysis::computePathTraces() {
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
    ExitingBlockPathIDMap.insert(make_pair(BB, i++));

  LLVM_DEBUG(errs() << "Number of Paths = " << ExitingBlockPathIDMap.size()
                    << "\n");

  // Mark each predecessor of path exiting block with path id.
  for (auto & KV : ExitingBlockPathIDMap)
    visitPredecessor(KV.getFirst(), KV.getSecond());

  prepareFinalMap();
}

void PacketPathAnalysis::visitPredecessor(BasicBlock * BB, unsigned PathID) {
  for (auto Pred : predecessors(BB)) {
    auto & PathIDSet = BlockToPathSet[Pred];
    if (PathIDSet.find(PathID) != PathIDSet.end())
      continue;

    PathIDSet.insert(PathID);
    PathIDToBLockList[PathID].push_back(Pred);
    visitPredecessor(Pred, PathID);
  }
}

void PacketPathAnalysis::prepareFinalMap() {
  // XXX If Block is part of more than one path, assign id = 0.
  // In predicate block, check for each path id can be done.
  // This way is simple for now.
  for (auto & KV : BlockToPathSet) {
    auto & V = KV.getSecond();
    if (V.size() > 1) {
      BlockToPathIdMap.insert(make_pair(KV.getFirst(), 0));
    } else {
      assert (V.size() == 1);
      BlockToPathIdMap.insert(make_pair(KV.getFirst(), *V.begin()));
    }
  }
  for (auto & KV : ExitingBlockPathIDMap) {
    BlockToPathIdMap.insert(make_pair(KV.getFirst(), KV.getSecond()));
  }
}

void PacketPathAnalysis::dumpDebugDataToConsole() {
  errs() << "Basic block path information\n";
  errs() << "Path Exiting Blocks\n";
  for (auto & E : ExitingBlockPathIDMap) {
    auto & BB = E.getFirst();
    BB->printAsOperand(errs());
    errs() << " : ";
    auto & ID = E.getSecond();
    errs() << to_string(ID) << "  ";
    errs() << "\n";
  }
  errs() << "Block to Path memebers map\n";
  for (auto & E : BlockToPathSet) {
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
}

}
