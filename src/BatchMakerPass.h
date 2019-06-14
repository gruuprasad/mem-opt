#ifndef TAS_BATCHMAKERPASS_H
#define TAS_BATCHMAKERPASS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

namespace {

struct TASBatchMaker : public llvm::FunctionPass {
  static char ID;
  TASBatchMaker() : FunctionPass(ID) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
 
  bool doInitialization(llvm::Module &M) override;

  bool runOnFunction(llvm::Function &F) override;
};

}

#endif
