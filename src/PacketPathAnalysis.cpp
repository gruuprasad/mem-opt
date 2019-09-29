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

void PacketPathAnalysis::calculate() {
  BlockToPathSet.clear();
  PathIDToBLockList.clear();

  RegionInfo RI;
  DF.analyze(DT);
  RI.recalculate(*F, &DT, &PDT, &DF);
  auto TR = RI.getTopLevelRegion();

  vector<Region *> RQ;
  for (auto & E : *TR) {
    RQ.push_back(E.get());
  }

  for (auto & R : RQ) {
    computePathTraces(R);
  }
}

void PacketPathAnalysis::computePathTraces(Region * R) {
  RegionEntry = R->getEntry();

  ExitingBlocks.clear();
  R->getExitingBlocks(ExitingBlocks);

  // Mark each predecessor of path exiting block with path id.
  for (auto & BB : ExitingBlocks) {
    BlockToPathIdMap.insert(make_pair(BB, ++PathIdCounter));
    visitPredecessor(BB, PathIdCounter);
  }

  prepareFinalMap();
}

void PacketPathAnalysis::visitPredecessor(BasicBlock * BB, unsigned PathID) {
  for (auto Pred : predecessors(BB)) {
    auto & PathIDSet = BlockToPathSet[Pred];
    if (PathIDSet.find(PathID) != PathIDSet.end())
      continue;

    PathIDSet.insert(PathID);
    PathIDToBLockList[PathID].push_back(Pred);

    if (Pred == RegionEntry) return;
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
}

void PacketPathAnalysis::dumpDebugDataToConsole() {
  errs() << "Basic block path information\n";
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
