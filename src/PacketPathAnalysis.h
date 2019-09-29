#ifndef PACKET_PATH_TRACE_ANALYSIS
#define PACKET_PATH_TRACE_ANALYSIS

#include <llvm/Analysis/DominanceFrontier.h>
#include <llvm/Analysis/PostDominators.h>
#include <llvm/Analysis/RegionInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Dominators.h>

namespace tas {

class PacketPathAnalysis {
  using IntToBasicBlocksMapType = llvm::DenseMap<unsigned,
                                  llvm::SmallVector<llvm::BasicBlock *, 4>>;
  using BasicBlockToIntegersMapType = llvm::DenseMap<llvm::BasicBlock *,
                                      llvm::DenseSet<unsigned>>;

  llvm::Function * F;
  llvm::DominatorTree DT;
  llvm::PostDominatorTree PDT;
  llvm::DominanceFrontier DF;
  int PathIdCounter = 0;

  llvm::BasicBlock * RegionEntry;
  BasicBlockToIntegersMapType BlockToPathSet;
  llvm::SmallVector<llvm::BasicBlock *, 4> ExitingBlocks;
  IntToBasicBlocksMapType PathIDToBLockList;
  llvm::DenseMap<llvm::BasicBlock *, unsigned> BlockToPathIdMap;

  void computePathTraces(llvm::Region * R);
  void visitPredecessor(llvm::BasicBlock * BB, unsigned PathID);
  void prepareFinalMap();

public:
  PacketPathAnalysis(llvm::Function * F_) :
    F(F_), DT(*F), PDT(*F), DF() {
    calculate();
  }

  void calculate();
  void dumpDebugDataToConsole();

  // Accessors
  unsigned getNumerOfPaths() { return PathIdCounter; }

  IntToBasicBlocksMapType & getPathSetRef() { return PathIDToBLockList; }

  llvm::DenseMap<llvm::BasicBlock *, unsigned> getBlockToPathIDMapRef() {
    return BlockToPathIdMap;
  }
};

}

#endif
