#ifndef TAS_BATCHMAKER_H
#define TAS_BATCHMAKER_H

#include <deque>
#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {

class TASForLoop;

struct TASArgAttr {
  std::string Prefix;
  bool IsBatch;
  int PosIdx;
  llvm::Type * Ty;
  llvm::Argument * Val;
  std::string Name;

  TASArgAttr(bool b, int p, llvm::Type * t, llvm::Argument * v, std::string n) :
    Prefix("batch_"), IsBatch(b), PosIdx(p),
    Ty(IsBatch ? llvm::PointerType::get(t, 0) : t),
    Val(v),
    Name(IsBatch ? Prefix + std::to_string(p) : n) {}
};

/// This pass converts the single packet processing function into
/// a function which processes a batch of packets.
class BatchMaker {
  llvm::Function * NonBatchFunc;
  llvm::Function * BatchFunc;
  llvm::SmallPtrSet<llvm::Value *, 4> ArgsToBatch;
  std::vector<TASArgAttr> BatchFuncArgList;
  llvm::IRBuilder<> Builder;
  unsigned BatchSize = 4;
  std::string BatchSizeVarName = std::string("TAS_BatchSize");
  std::string ReturnVarName = std::string("TAS_ReturnVar");

  /// Handles to batch related variables.
  llvm::AllocaInst * IdxPtr;
  llvm::AllocaInst * RetAlloca;
  llvm::Instruction * Return;

  void createBatchedFormFnPrototype();
  void updateBasicBlocksInBatchFunc();
  void addBatchLoop();

  public:
  BatchMaker(llvm::Function * F_) :
    NonBatchFunc(F_), BatchFunc(nullptr),
    Builder(NonBatchFunc->getContext()) {}

  bool run();
}; // tas namespace

}

#endif
