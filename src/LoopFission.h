#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

namespace tas {

class LoopFission {
  llvm::Function * F;
  llvm::LoopInfo * LI;

public:
  LoopFission(llvm::Function * F_, llvm::LoopInfo * LI_) :
    F(F_), LI(LI_) {}

  bool run();
};

} // tas namespace

namespace {

struct TASLoopFission : public llvm::FunctionPass {
  static char ID;
  TASLoopFission() : FunctionPass(ID) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
  
  bool doInitialization(llvm::Module &M) override;

  // Perform loop-fission on function annotated with tas attribute.
  bool runOnFunction(llvm::Function &F) override;
};

} // Anonymous namespace
