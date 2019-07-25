#ifndef TAS_BATCHPROCESSPASS_H
#define TAS_BATCHPROCESSPASS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

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
