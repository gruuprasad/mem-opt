#ifndef TAS_BATCHPROCESS_H
#define TAS_BATCHPROCESS_H

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Pass.h>


namespace tas {

/// For loop construct containing single logical body
class TASForLoop {
  llvm::BasicBlock * PreHeader;
  llvm::BasicBlock * Header;
  llvm::BasicBlock * Latch;
  llvm::BranchInst * ExitInst;
  llvm::Function * F;
  std::string Name;

  // Constructor
  explicit TASForLoop(llvm::LLVMContext & Ctx,
      llvm::BasicBlock * Prev, llvm::BasicBlock * Next,
      std::string & Name, llvm::Function * F = nullptr);

public:
  static TASForLoop * Create(llvm::LLVMContext & Ctx,
      llvm::BasicBlock * Prev, llvm::BasicBlock * Next,
      std::string Name = std::string(), 
      llvm::Function * F = nullptr) {
    return new TASForLoop(Ctx, Prev, Next, Name, F);
  }

  void addEmptyLoop(llvm::LLVMContext & Ctx, llvm::BasicBlock * Prev, llvm::BasicBlock * Next);
  void setLoopBody(llvm::BasicBlock * BodyBB);
  llvm::BasicBlock * getLatchBlock() {
    return Latch;
  }

  llvm::BasicBlock * getLatch() {
    return Latch;
  }
  llvm::BasicBlock * getHeader() {
    return Header;
  }
};

class BatchProcess {
  llvm::Function * F;
  llvm::LoopInfo * LI;
  llvm::SmallVector<llvm::Value *, 4> AnnotatedVariables;
  llvm::SmallVector<llvm::Instruction *, 4> AnnotatedVariableDefPoints;
  llvm::Value * InductionVariable;

  public:
  BatchProcess(llvm::Function * F_, llvm::LoopInfo * LI_) :
    F(F_), LI(LI_) {}

  bool run();
  void addEmptyLoop(llvm::BasicBlock * InsertBefore);
};

} // tas namespace

namespace {

struct TASBatchProcess : public llvm::FunctionPass {
  static char ID;
  TASBatchProcess() : FunctionPass(ID) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
 
  bool doInitialization(llvm::Module &M) override;

  bool runOnFunction(llvm::Function &F) override;
};

}

#endif
