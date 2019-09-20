#include "PathDetector.hpp"

#include <llvm/Analysis/PostDominators.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Debug.h>

#include <cassert>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "tas"

namespace tas {

void PathDetector::visitPredecessor(const BasicBlock * BB, unsigned PathID) {
  for (auto Pred : predecessors(BB)) {
    if (BlockToPathIdMap[Pred].find(PathID) != BlockToPathIdMap[Pred].end())
      continue;
    BlockToPathIdMap[Pred].insert(PathID);
    PathIDToBLockList[PathID].push_back(Pred);
    visitPredecessor(Pred, PathID);
  }
}

void PathDetector::DetectExitingBlocks() {
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
  const auto * ReturnBlock = ReturnBlocks.front();

  int i = 1;
  for (auto * BB : predecessors(ReturnBlock))
    PathExitingBlocksToPathIDMap.insert(make_pair(BB, i++));

  LLVM_DEBUG(errs() << "Number of Paths = " << PathExitingBlocksToPathIDMap.size() << "\n");

  // Mark each predecessor of path exiting block with path id.
  for (const auto & KV : PathExitingBlocksToPathIDMap) {
    const auto * EB = KV.getFirst();
    auto PathID = KV.getSecond(); 
    visitPredecessor(EB, PathID);
  }

 // dumpDebugDataToConsole();
}

void PathDetector::dumpDebugDataToConsole() {
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
  for (auto & E : BlockToPathIdMap) {
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
