#ifndef TAS_FORLOOP_H
#define TAS_FORLOOP_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {


/// For loop construct containing single logical body
class TASForLoop {
  llvm::BasicBlock * PreHeader;
  llvm::BasicBlock * Header;
  llvm::BasicBlock * EntryBody;
  llvm::BasicBlock * ExitingBody;
  llvm::BasicBlock * Latch;
  llvm::BranchInst * ExitInst;
  llvm::PHINode * IndexVar;
  llvm::Function * F;
  std::string Name;
  static const unsigned BATCH_SIZE = 32;
  llvm::Value * TripCount;

public:
// Constructor
  explicit TASForLoop(llvm::LLVMContext & Ctx,
      llvm::BasicBlock * Prev, llvm::BasicBlock * Next,
      const std::string & Name, llvm::Function * F = nullptr, llvm::Value * TC = nullptr);

  static TASForLoop * Create(llvm::LLVMContext & Ctx,
      llvm::BasicBlock * Prev, llvm::BasicBlock * Next,
      std::string Name = std::string(), 
      llvm::Function * F = nullptr, llvm::Value * TC = nullptr) {
    return new TASForLoop(Ctx, Prev, Next, Name, F, TC);
  }

  void addEmptyLoop(llvm::LLVMContext & Ctx, llvm::BasicBlock * Prev, llvm::BasicBlock * Next);
  void setLoopBody(llvm::BasicBlock * BodyBB);
  void setLoopBody(llvm::BasicBlock * EntryBB, llvm::BasicBlock * ExitingBB);

  llvm::BasicBlock * getLatchBlock() {
    return Latch;
  }

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
};

}

#endif
