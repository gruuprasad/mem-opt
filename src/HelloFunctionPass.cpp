#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {

struct HelloFunction : public FunctionPass {
  static char ID;
  HelloFunction() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "Hello: ";
    errs().write_escaped(F.getName()) << "\n";
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};

char HelloFunction::ID = 0;
static RegisterPass<HelloFunction> X("hello", "Hello Function Pass",
                                      false /* Only looks at CFG */,
                                      false /* Analysis Pass */);
} // Anonymous namespace
