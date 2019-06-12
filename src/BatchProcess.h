#ifndef TAS_BATCHPROCESS_H
#define TAS_BATCHPROCESS_H

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {

class TASForLoop;

class BatchProcess {
  llvm::Function * F;
  llvm::LoopInfo * LI;
  llvm::DominatorTree * DT;
  llvm::SmallVector<llvm::Value *, 4> AnnotatedVariables;
  llvm::SmallVector<llvm::Instruction *, 4> AnnotatedVariableDefPoints;
  llvm::SmallVector<llvm::Value *, 4> PrefetchAddresses;
  llvm::Value * InductionVariable;

  public:
  BatchProcess(llvm::Function * F_, llvm::LoopInfo * LI_, llvm::DominatorTree * DT_) :
    F(F_), LI(LI_), DT(DT_) {}

  bool run();
  void addEmptyLoop(llvm::BasicBlock * InsertBefore);
  void detectAnnotatedVariable();
  void findVariableUsePoints();
  void insertPrefetchCalls();
  void splitLoop(llvm::Loop * L0);
  void fixValueDependenceBetWeenLoops(TASForLoop * NewLoop, llvm::Value * OldIndex);
  llvm::Value * createArray(llvm::Type * Ty, unsigned size);
};

} // tas namespace

#endif
