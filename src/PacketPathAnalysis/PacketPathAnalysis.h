#ifndef PACKET_PATH_TRACE_ANALYSIS
#define PACKET_PATH_TRACE_ANALYSIS

#include <llvm/IR/Function.h>
#include <llvm/IR/Dominators.h>

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

  void computePathTraces();
  void visitPredecessor(llvm::BasicBlock * BB, unsigned PathID);
  void dumpDebugDataToConsole();

  void prepareFinalMap();

public:
  PacketPathAnalysis(llvm::Function * F_) : F(F_), EntryBlock(&F->getEntryBlock()) {
    computePathTraces();
  }

  void recalculate();

  // Accessors
  unsigned getNumerOfPaths() { return PathExitingBlocksToPathIDMap.size(); }

  IntToBasicBlocksMapType & getPathSetRef() { return PathIDToBLockList; }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> & getPathExitingBlockListRef()
  {
    return PathExitingBlocksToPathIDMap;
  }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> getBlockToPathIDMapRef() {
    return BlockToPathIdMap;
  }

  llvm::BasicBlock * getReturnBlock() { return ReturnBlock; }
};

}

#endif
