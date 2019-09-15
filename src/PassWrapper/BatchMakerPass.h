#ifndef TAS_BATCHMAKERPASS_H
#define TAS_BATCHMAKERPASS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

namespace {

struct TASBatchMaker : public llvm::ModulePass {
  static char ID;
  TASBatchMaker() : ModulePass(ID) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
 
  bool runOnModule(llvm::Module &M) override;
};

}

namespace tas {

void runBatchMakerPass(llvm::Module * M);

}

#endif
