#ifndef TAS_BATCHPROCESS_H
#define TAS_BATCHPROCESS_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Pass.h>


namespace tas {

class BatchProcess {
  llvm::Function * F;
  llvm::LoopInfo * LI;
  llvm::SmallVector<llvm::Value *, 4> AnnotatedVariables;

public:
  BatchProcess(llvm::Function * F_, llvm::LoopInfo * LI_) :
    F(F_), LI(LI_) {}

  bool run();
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