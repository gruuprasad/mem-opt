#ifndef PACKET_PATH_TRACE_ANALYSIS
#define PACKET_PATH_TRACE_ANALYSIS

#include <llvm/IR/Function.h>

namespace tas {

class PacketPathAnalysis {
  using IntToBasicBlocksMapType = llvm::DenseMap<unsigned, llvm::SmallVector<const llvm::BasicBlock *, 4>>;
  using BasicBlockToIntegersMapType = llvm::DenseMap<const llvm::BasicBlock *, llvm::DenseSet<unsigned>>;

  llvm::Function * F;
  BasicBlockToIntegersMapType BlockToPathIdMap;
  llvm::DenseMap<const llvm::BasicBlock *, unsigned> PathExitingBlocksToPathIDMap;
  IntToBasicBlocksMapType PathIDToBLockList;

  void dumpDebugDataToConsole();

public:
  PacketPathAnalysis(llvm::Function * F_) : F(F_) {
    computePathTrace();
  }

  void recalculate();

  void computePathTrace();

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
