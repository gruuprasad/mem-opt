#ifndef TAS_LOOPSPLITTER_H
#define TAS_LOOPSPLITTER_H

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Function.h>
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

class LoopSplitter {
  llvm::Function * F;
  llvm::LoopInfo * LI;
  llvm::SmallVector<llvm::Value *, 4> AnnotatedVariables;
  llvm::SmallVector<llvm::Instruction *, 4> AnnotatedVariableDefPoints;
  llvm::SmallVector<llvm::Value *, 4> PrefetchAddresses;
  llvm::Value * InductionVariable;

  public:
  LoopSplitter(llvm::Function * F_, llvm::LoopInfo * LI_)
    : F(F_), LI(LI_) {}

  bool run();
  unsigned getAnnotatedVarCount() { return AnnotatedVariables.size(); }
  void addEmptyLoop(llvm::BasicBlock * InsertBefore);
  void findVariableUsePoints();
  void splitLoop(llvm::Loop * L0);
  void fixValueDependenceBetWeenLoops(TASForLoop * NewLoop, llvm::Value * OldIndex);
};

} // tas namespace

#endif
