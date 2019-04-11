#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

namespace tas {

/**
 * Steps:
 * 1. Detect and store all variables marked with prefetch annotation.
 * 2. Detect loop trip count.
 * 3. Detect prefetch variables def instructions.
 * 4. Find first access of prefetch variable value access. Aim is to make this
 *    access result in cache hit.
 *
 */

class LoopFission {
  llvm::Function * F;
  llvm::LoopInfo * LI;

  bool ArePreconditionsMet();

public:
  LoopFission(llvm::Function * F_, llvm::LoopInfo * LI_) :
    F(F_), LI(LI_) {}

  bool run();
};


void detectPrefetchVariables(llvm::Function * F, llvm::SmallVectorImpl<llvm::Value *> & EI);
int detectLoopTripCount();
llvm::Value * detectPrefetchVariableDef(llvm::Value * PV);
void collectPrefetchVariablesUses(llvm::SmallVectorImpl<llvm::Value *> & PrefetchUses,
                                  llvm::SmallVectorImpl<llvm::Value *> & PrefetechVariables);

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
