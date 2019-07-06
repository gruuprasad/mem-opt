#ifndef TAS_BATCHMAKER_H
#define TAS_BATCHMAKER_H

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
  llvm::SmallVector<llvm::Value *, 4> PrefetchVars;

  public:
  BatchMaker(llvm::Function * F_) :
    OldFunc(F_), NewFunc(nullptr), PrefetchVars() {}

  bool run();
  void createBatchedFormFn();
}; // tas namespace

}

#endif
