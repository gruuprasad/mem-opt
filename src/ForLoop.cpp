#include "ForLoop.h"
#include "Util.h"

#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace tas {

TASForLoop::TASForLoop(LLVMContext & Ctx, BasicBlock * Prev,
    BasicBlock * Next, const std::string & Name, Function * F,
    llvm::AllocaInst * TC, AllocaInst * IP)
  : F(F), Name (std::move(Name)), TripCount(TC), IdxVarPtr(IP)
{
  addEmptyLoop(Ctx, Prev, Next);
}

void TASForLoop::addEmptyLoop(LLVMContext & Ctx, BasicBlock * Prev,
                              BasicBlock * Next) {
  PreHeader = BasicBlock::Create(Ctx, Name + ".preheader", F, Next);
  Header = BasicBlock::Create(Ctx, Name + ".header", F, Next);
  Latch = BasicBlock::Create(Ctx, Name + ".latch", F, Next);

  IRBuilder<> Builder(PreHeader);
  Builder.CreateStore(Builder.getInt32(0), IdxVarPtr);
  Builder.CreateBr(Header);

  Builder.SetInsertPoint(Latch);
  assert (IdxVarPtr != nullptr && "Index variable ptr can't be null");
  auto BI = Builder.CreateBr(Header);
  addIncrementIndexOp(IdxVarPtr, BI);

  Builder.SetInsertPoint(Header);
  auto TCVal = Builder.CreateLoad(TripCount);
  IndexVar = Builder.CreateLoad(IdxVarPtr);
  auto * icmp = Builder.CreateICmpSLT(IndexVar, TCVal, "loop-predicate");

  // Stitch entry point in control flow.
  if (Prev) {
    if (Prev->getTerminator()->getNumOperands() == 3) 
      Prev->getTerminator()->setSuccessor(1, PreHeader);
    else
      Prev->getTerminator()->setSuccessor(0, PreHeader);
  }

  assert (Next != nullptr && "Exit block must be specified");
  ExitInst = Builder.CreateCondBr(icmp, Latch, Next);
}

void TASForLoop::setLoopBody(BasicBlock * BodyBB) {
  EntryBody = ExitingBody = BodyBB;
  Header->getTerminator()->setSuccessor(0, BodyBB);
  BodyBB->getTerminator()->setSuccessor(0, Latch);
}

void TASForLoop::setLoopBody(BasicBlock * EntryBB,
                             BasicBlock * ExitingBB) {
  EntryBody = EntryBB;
  ExitingBody = ExitingBB;
  Header->getTerminator()->setSuccessor(0, EntryBody);
  ExitingBody->getTerminator()->setSuccessor(0, Latch);
}

}
