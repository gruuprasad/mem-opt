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
  BasicBlockToIntegersMapType BlockToPathSet;
  llvm::DenseMap<llvm::BasicBlock *, unsigned> ExitingBlockPathIDMap;
  IntToBasicBlocksMapType PathIDToBLockList;
  llvm::DenseMap<llvm::BasicBlock *, unsigned> BlockToPathIdMap;

  void computePathTraces();
  void visitPredecessor(llvm::BasicBlock * BB, unsigned PathID);
  void prepareFinalMap();

public:
  PacketPathAnalysis(llvm::Function * F_) : F(F_), EntryBlock(&F->getEntryBlock()) {
    computePathTraces();
  }

  void recalculate();
  void dumpDebugDataToConsole();

  // Accessors
  unsigned getNumerOfPaths() { return ExitingBlockPathIDMap.size(); }

  IntToBasicBlocksMapType & getPathSetRef() { return PathIDToBLockList; }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> & getPathExitingBlockListRef()
  {
    return ExitingBlockPathIDMap;
  }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> getBlockToPathIDMapRef() {
    return BlockToPathIdMap;
  }

  llvm::BasicBlock * getReturnBlock() { return ReturnBlock; }
};

}

#endif
