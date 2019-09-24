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

/// This pass converts the single packet processing function into
/// a function which processes a batch of packets.
class BatchMaker {
  llvm::Function * NonBatchFunc;
  llvm::Function * BatchFunc;
  llvm::SmallPtrSet<llvm::Value *, 4> ArgsToBatch;

  llvm::SmallVector<llvm::Type *, 4> BatchArgTypes;
  llvm::SmallVector<std::string, 4> BatchArgNames;
  llvm::SmallVector<llvm::Value *, 4> BatchArgs;
  llvm::SmallVector<llvm::Value *, 4> BatchGEPs;
  llvm::SmallPtrSet<llvm::Value *, 4> BatchAllocas;
  std::deque<unsigned> BatchParamIndices;
  llvm::AllocaInst * IndexVarPtr;
  llvm::Argument * RetArg;
  llvm::BasicBlock * EntryBB;

  void replaceArgUsesWithBatchArgVal();

  public:
  BatchMaker(llvm::Function * F_) :
    NonBatchFunc(F_), BatchFunc(nullptr) {}

  bool run();
  void createBatchedFormFnPrototype();
  void updateBasicBlocksInBatchFunc();
  void setArgumentNamesInBatchFunc();
  void fillBasicBlocksInBatchFunc();
}; // tas namespace

}

#endif
