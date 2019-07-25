#ifndef TAS_BATCHMAKER_H
#define TAS_BATCHMAKER_H

#include <deque>
#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {

class TASForLoop;

class BatchMaker {
  llvm::Function * OldFunc;
  llvm::Function * NewFunc;
  llvm::SmallPtrSet<llvm::Value *, 4> ArgsToBatch;
  llvm::SmallVector<llvm::Value *, 4> BatchedArgs;
  llvm::SmallVector<llvm::Value *, 4> PrefetchVars;
  llvm::SmallVector<llvm::Type *, 4> NewParams;
  llvm::SmallVector<std::string, 4> ArgNames;
  std::deque<unsigned> BatchParamIndices;
  llvm::Value * RetParam;
  llvm::BasicBlock * BatchCodeStartBlock;
  llvm::SmallPtrSet<llvm::Value *, 4> BatchedAllocas;
  llvm::SmallVector<llvm::Value *, 4> BatchGEPs;
  llvm::AllocaInst * RetAlloca;

  public:
  BatchMaker(llvm::Function * F_) :
    OldFunc(F_), NewFunc(nullptr), PrefetchVars() {}

  bool run();
  void createBatchedFormFn();
  void constructBatchParameters();
  llvm::Function * createBatchFunction();
  void setArgumentNames();
  void setFunctionBody();
}; // tas namespace

}

#endif
