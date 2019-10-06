#ifndef TAS_LOOPSPLITTERPASS_H
#define TAS_LOOPSPLITTERPASS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

namespace {

struct TASLoopSplitter : public llvm::FunctionPass {
  static char ID;
  TASLoopSplitter() : FunctionPass(ID) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
 
  bool doInitialization(llvm::Module &M) override;

  bool runOnFunction(llvm::Function &F) override;
};

}

#endif
