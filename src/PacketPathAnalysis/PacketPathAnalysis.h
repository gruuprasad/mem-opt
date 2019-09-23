#ifndef PACKET_PATH_TRACE_ANALYSIS
#define PACKET_PATH_TRACE_ANALYSIS

#include <llvm/IR/Function.h>

namespace tas {

class PacketPathAnalysis {
  using IntToBasicBlocksMapType = llvm::DenseMap<unsigned, llvm::SmallVector<llvm::BasicBlock *, 4>>;
  using BasicBlockToIntegersMapType = llvm::DenseMap<llvm::BasicBlock *, llvm::DenseSet<unsigned>>;

  llvm::Function * F;
  llvm::BasicBlock * EntryBlock;
  llvm::BasicBlock * ReturnBlock;
  BasicBlockToIntegersMapType IntermediateBBPathIdMap;
  llvm::DenseMap<llvm::BasicBlock *, unsigned> PathExitingBlocksToPathIDMap;
  IntToBasicBlocksMapType PathIDToBLockList;
  llvm::DenseMap<llvm::BasicBlock *, unsigned> BlockToPathIdMap;
  llvm::DenseMap<llvm::BasicBlock *, unsigned> MiddleBlockToPathIdMap;
  llvm::SmallVector<llvm::BasicBlock *, 16> TotalOrder;
  llvm::SmallVector<llvm::BasicBlock *, 8> PathOrder;

  void prepareFinalMap();

public:
  PacketPathAnalysis(llvm::Function * F_) : F(F_), EntryBlock(&F->getEntryBlock()) {
    computePathTrace();
  }

  void recalculate();

  void computePathTrace();

  void dumpDebugDataToConsole();

  void visitPredecessor(llvm::BasicBlock * BB, unsigned PathID);

  unsigned getNumerOfPaths() { return PathExitingBlocksToPathIDMap.size(); }

  IntToBasicBlocksMapType & getPathSetRef() { return PathIDToBLockList; }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> & getPathExitingBlockListRef()
  {
    return PathExitingBlocksToPathIDMap;
  }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> getBlockToPathIDMapRef() {
    return BlockToPathIdMap;
  }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> & getIntermediatePathMap() {
    return MiddleBlockToPathIdMap;
  }

  llvm::BasicBlock * getReturnBlock() { return ReturnBlock; }
};

}

#endif
