#ifndef TAS_FORLOOP_H
#define TAS_FORLOOP_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {

// TODO Rewrite this class with clean interface.
/// For loop construct containing single logical body
class TASForLoop {
  llvm::Function * F;
  std::string Name;
  llvm::Value * TripCount;
  llvm::AllocaInst * IdxVarPtr;
  llvm::Value * IndexVar;
  static const unsigned BATCH_SIZE = 32;
  llvm::BasicBlock * PreHeader;
  llvm::BasicBlock * Header;
  llvm::BasicBlock * EntryBody;
  llvm::BasicBlock * ExitingBody;
  llvm::BasicBlock * Latch;
  llvm::BranchInst * ExitInst;

public:
  // Constructor
  explicit TASForLoop(llvm::LLVMContext & Ctx,
      llvm::BasicBlock * Prev, llvm::BasicBlock * Next,
      const std::string & Name, llvm::Function * F = nullptr,
      llvm::Value * TC = nullptr, llvm::AllocaInst * IP = nullptr);

  void addEmptyLoop(llvm::LLVMContext & Ctx, llvm::BasicBlock * Prev, llvm::BasicBlock * Next);
  void setLoopBody(llvm::BasicBlock * BodyBB);
  void setLoopBody(llvm::BasicBlock * EntryBB, llvm::BasicBlock * ExitingBB);

  llvm::BasicBlock * getLatch() {
    return Latch;
  }

  llvm::BasicBlock * getHeader() {
    return Header;
  }

  llvm::Value * getIndexVariable() {
    return IndexVar;
  }

  llvm::BasicBlock * getBody() {
    if (EntryBody != ExitingBody)
      return nullptr;
    return EntryBody;
  }

  static unsigned getLoopTripCount() {
    return BATCH_SIZE;
  }

  llvm::Value * getLoopTripCountValue() {
    return TripCount;
  }
};

}

#endif
