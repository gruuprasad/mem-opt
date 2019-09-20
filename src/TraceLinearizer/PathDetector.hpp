#ifndef PATH_DETECTOR_HPP
#define PATH_DETECTOR_HPP

#include <llvm/IR/Function.h>

namespace tas {

class PathDetector {
  using IntToBasicBlocksMapType = llvm::DenseMap<unsigned, llvm::SmallVector<const llvm::BasicBlock *, 4>>;
  using BasicBlockToIntegersMapType = llvm::DenseMap<const llvm::BasicBlock *, llvm::DenseSet<unsigned>>;

  llvm::Function * F;
  BasicBlockToIntegersMapType BlockToPathIdMap;
  llvm::DenseMap<const llvm::BasicBlock *, unsigned> PathExitingBlocksToPathIDMap;
  IntToBasicBlocksMapType PathIDToBLockList;

  void dumpDebugDataToConsole();

public:
  PathDetector(llvm::Function * F_) : F(F_) {}

  void DetectExitingBlocks();

  void visitPredecessor(const llvm::BasicBlock * BB, unsigned PathID);

  unsigned getNumerOfPaths() { return PathExitingBlocksToPathIDMap.size(); }

  IntToBasicBlocksMapType & getPathSetRef() { return PathIDToBLockList; }

  llvm::DenseMap<const llvm::BasicBlock *, unsigned> & getPathExitingBlockListRef()
  {
    return PathExitingBlocksToPathIDMap;
  }

  BasicBlockToIntegersMapType getBlockToPathIDMapRef() {
    return BlockToPathIdMap;
  }
};

}

#endif
