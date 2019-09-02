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

/// This pass converts the single packet processing function into
/// a function which processes a batch of packets.
class BatchMaker {
  llvm::Function * NonBatchFunc;
  llvm::Function * BatchFunc;
  llvm::SmallVector<llvm::Value *, 4> PrefetchVars;
  llvm::SmallPtrSet<llvm::Value *, 4> ArgsToBatch;

  public:
  BatchMaker(llvm::Function * F_) :
    NonBatchFunc(F_), BatchFunc(nullptr) {}

  bool run();
  void createBatchedFormFn();
}; // tas namespace

}

#endif
