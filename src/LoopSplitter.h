#ifndef TAS_LOOPSPLITTER_H
#define TAS_LOOPSPLITTER_H

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace llvm {
class LoopInfo;
class Loop;
class AllocaInst;
}

namespace tas {

class TASForLoop;
llvm::AllocaInst * getLoopIndexVar(llvm::Loop * L);

class LoopSplitter {
  llvm::Function * F;
  llvm::LoopInfo * LI;

  public:
  LoopSplitter(llvm::Function * F_, llvm::LoopInfo * LI_)
    : F(F_), LI(LI_) {}

  bool run();
};

} // tas namespace

#endif
