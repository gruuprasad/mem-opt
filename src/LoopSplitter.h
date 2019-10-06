#ifndef TAS_LOOPSPLITTER_H
#define TAS_LOOPSPLITTER_H

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {

class TASForLoop;

class LoopSplitter {
  llvm::Function * F;
  llvm::LoopInfo * LI;
  llvm::SmallVector<llvm::Value *, 4> AnnotatedVariables;
  llvm::SmallVector<llvm::Instruction *, 4> AnnotatedVariableDefPoints;
  llvm::SmallVector<llvm::Value *, 4> PrefetchAddresses;
  llvm::Value * InductionVariable;

  public:
  LoopSplitter(llvm::Function * F_, llvm::LoopInfo * LI_, llvm::DominatorTree * DT_)
    : F(F_), LI(LI_) {}

  bool run();
  void addEmptyLoop(llvm::BasicBlock * InsertBefore);
  void findVariableUsePoints();
  void splitLoop(llvm::Loop * L0);
  void fixValueDependenceBetWeenLoops(TASForLoop * NewLoop, llvm::Value * OldIndex);
};

} // tas namespace

#endif
