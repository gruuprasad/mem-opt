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
  llvm::Function * F;
  llvm::LoopInfo * LI;
  Stats stat;
  llvm::SwitchInst * SwitchI;

  llvm::BasicBlock * ExitBlock;
  llvm::DenseMap<const llvm::BasicBlock *, llvm::ConstantInt *> BBToId;
  llvm::SmallVector<llvm::BasicBlock *, 4> LoopSplitEdgeBlocks;
  bool prepareForLoopSplit(llvm::Function * F, llvm::Loop * L0, Stats & stat);
public:
  LoopSplitter(llvm::Function * F_, llvm::LoopInfo * LI_)
    : F(F_), LI(LI_) {}

  bool run();
  Stats & getStats() { return stat; }
  void addAdapterBasicBlocks(llvm::Instruction * SP, llvm::Value * Idx);
  void doLoopSplit(llvm::Function * F, llvm::Loop * L0, llvm::BasicBlock * SplitBlock);
};

} // tas namespace

#endif
