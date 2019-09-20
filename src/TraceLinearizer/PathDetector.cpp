#include "PathDetector.hpp"

#include <llvm/Analysis/PostDominators.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>

#include <cassert>

using namespace llvm;
using namespace std;

namespace tas {

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

  DenseMap<const BasicBlock *, unsigned> PathExitingBlocksToPathID;
  int i = 1;
  // Note: Keep id = 0 for the root path, starting from entry basic block
  // until first split point. Since root path is executed for all the paths,
  // no need to add condition. Later when we find out that if basic block has
  // all the path id, then we can replace multiple condition with single condition.
  for (auto * BB : predecessors(ReturnBlock))
    PathExitingBlocksToPathID.insert(make_pair(BB, i++));

  errs() << "Number of Paths = " << PathExitingBlocksToPathID.size() << "\n";

  // Each basic block can belong to either one path or multiple
  // paths. For example, entry block belongs to all the paths.
  // We identify for each basic block, number of paths it belongs to.
  DenseMap<BasicBlock *, SmallVector<unsigned, 4>> BlockToPaths;
  for (BasicBlock & BB : *F) {
    auto BE = BasicBlockEdge(&EntryBlock, &BB);
    for (const auto & PathExitBlock : PathExitingBlocksToPathID) {
      if (PDT.dominates(PathExitBlock.getFirst(), &BB)) {
        BlockToPaths[&BB].push_back(PathExitBlock.getSecond());
        continue;
      }

      if (DT.dominates(BE, PathExitBlock.getFirst())) {
        BlockToPaths[&BB].push_back(PathExitBlock.getSecond());
        continue;
      }
    }
  }

  errs() << "Basic block path information\n";
  for (auto & E : BlockToPaths) {
    auto & BB = E.getFirst();
    BB->printAsOperand(errs());
    errs() << " : ";
    auto & IDs = E.getSecond();
    for (auto & ID : IDs) {
      errs() << to_string(ID) << "  ";
    }
    errs() << "\n";
  }
}

}
