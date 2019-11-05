#ifndef TAS_LOOPSPLITTER_H
#define TAS_LOOPSPLITTER_H

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace llvm {
class LoopInfo;
class Loop;
class AllocaInst;
}

namespace tas {

class TASForLoop;
llvm::AllocaInst * getLoopIndexVar(llvm::Loop * L);

struct Stats {
  unsigned AnnotatedVarsSize;
  unsigned VarUsePointsSize;

  Stats() = default;
  Stats(unsigned A, unsigned V) :
    AnnotatedVarsSize(A), VarUsePointsSize(V) {}
  Stats(const Stats & o) = default;
};

class LoopSplitter {
  using ListOfBlocksType = std::vector<std::vector<llvm::BasicBlock *>>; 
  llvm::Function * F;
  llvm::LoopInfo * LI;
  Stats stat;
  llvm::SwitchInst * SwitchI;

  llvm::SmallVector<llvm::Value *, 4> AnnotatedVars;
  llvm::SmallVector<llvm::LoadInst *, 4> VarUsePoints;
  llvm::SmallVector<llvm::Instruction *, 4> SplitPoints;
  llvm::SmallVector<llvm::BasicBlock *, 4> EndBlocks;
  llvm::BasicBlock * BodyEnd;
  llvm::SmallVector<std::pair<llvm::BasicBlock *, llvm::BasicBlock *>, 4> LoopBodyRange;
  llvm::BasicBlock * ExitBlock;
  llvm::DenseMap<const llvm::BasicBlock *, llvm::ConstantInt *> BBToId;
  llvm::SmallVector<llvm::BasicBlock *, 4> LoopSplitEdgeBlocks;
  ListOfBlocksType LoopBlocks;
  bool prepareForLoopSplit(llvm::Function * F, Stats & stat);
  void fixValueDependenceBetWeenLoops();
  void addBatchArrayForIntermediateVars(llvm::Loop * L0);
  void traverseLoopBody(std::vector<llvm::BasicBlock *> & LoopBlocks, llvm::BasicBlock * Start);
public:
  LoopSplitter(llvm::Function * F_, llvm::LoopInfo * LI_)
    : F(F_), LI(LI_) {}

  bool run();
  Stats & getStats() { return stat; }
  void addAdapterBasicBlocks(llvm::Loop * L0, llvm::Instruction * SP, llvm::Value * Idx);
};

} // tas namespace

#endif
