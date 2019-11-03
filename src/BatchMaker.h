#ifndef TAS_BATCHMAKER_H
#define TAS_BATCHMAKER_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <deque>
#include <string>

namespace tas {

struct TASArgAttr {
  std::string Prefix;
  bool IsBatch;
  int PosIdx;
  llvm::Type * Ty;
  llvm::Argument * Val;
  std::string Name;
  bool IsDoublePtr; // Only true when batched double pointer.

private:
  llvm::Type * setArgType(bool isBatch, llvm::Type * In) {
    if (isBatch) {
      IsDoublePtr = In->isPointerTy() && In->getContainedType(0)->isPointerTy();
      if (IsDoublePtr) return In;
      return llvm::PointerType::get(In, 0);
    }
    IsDoublePtr = false;
    return In;
  }

public:
  TASArgAttr(bool b, int p, llvm::Type * t, llvm::Argument * v, std::string n) :
    Prefix("batch_"), IsBatch(b), PosIdx(p),
    Ty(setArgType(IsBatch, t)),
    Val(v),
    Name(IsBatch ? Prefix + std::to_string(p) : n) {}
};

/// This pass converts the single packet processing function into
/// a function which processes a batch of packets.
class BatchMaker {
  llvm::Function * NonBatchFunc;
  llvm::Function * BatchFunc;
  llvm::SmallPtrSet<llvm::Value *, 4> ArgsToBatch;
  llvm::IRBuilder<> Builder;
  unsigned BatchSize = 4;
  bool IsRetTyVoid = false;
  std::string BatchSizeVarName = std::string("TAS_BatchSize");
  std::string ReturnVarName = std::string("TAS_ReturnVar");
  unsigned BatchSizeArgPos;
  llvm::AllocaInst * BatchSizeAlloca;

  void createBatchedFormFnPrototype(std::vector<TASArgAttr> & BatchFuncArgList);
  void addBatchLoop(llvm::BasicBlock * RetBlock, llvm::AllocaInst * IdxPtr);
  void replaceOldArgUsesWithBatchArgs(std::vector<TASArgAttr> & BatchFuncArgList,
                                      llvm::AllocaInst * IdxPtr);
  llvm::BasicBlock * storeRetValInPtrArg(llvm::Argument * RetArg,
                                         llvm::AllocaInst * IdxPtr);
  void doBatchTransform();

  public:
  BatchMaker(llvm::Function * F_) :
    NonBatchFunc(F_), BatchFunc(nullptr),
    Builder(NonBatchFunc->getContext()) {}

  llvm::Function * run();
}; // tas namespace

}

#endif
