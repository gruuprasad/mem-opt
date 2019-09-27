#include "ForLoop.h"

#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace tas {

TASForLoop::TASForLoop(LLVMContext & Ctx, BasicBlock * Prev,
    BasicBlock * Next, const std::string & Name, Function * F, llvm::Value * TC, AllocaInst * IP)
  : F(F), Name (std::move(Name)), TripCount(TC), IdxVarPtr(IP)
{
  addEmptyLoop(Ctx, Prev, Next);
}

void TASForLoop::addEmptyLoop(LLVMContext & Ctx, BasicBlock * Prev, BasicBlock * Next) {
  PreHeader = BasicBlock::Create(Ctx, Name + ".preheader", F, Next);
  Header = BasicBlock::Create(Ctx, Name + ".header", F, Next);
  Latch = BasicBlock::Create(Ctx, Name + ".latch", F, Next);

  // Update phi node edge if any
  IRBuilder<> Builder(Next);
  auto * PN = &*(Next->phis().begin());
  if (PN)
    PN->addIncoming(Builder.getInt32(0), Header);

  Builder.SetInsertPoint(PreHeader);
  Builder.CreateBr(Header);

  Builder.SetInsertPoint(Latch);
  assert (IdxVarPtr != nullptr && "Index variable ptr can't be null");
  IndexVar = Builder.CreateLoad(IdxVarPtr);
  auto IVNext = Builder.CreateAdd(IndexVar, Builder.getInt32(1));
  Builder.CreateStore(IVNext, IdxVarPtr);
  Builder.CreateBr(Header);

  Builder.SetInsertPoint(Header);
  IndexVar = Builder.CreateLoad(IdxVarPtr);
  auto * icmp = Builder.CreateICmpSLT(IndexVar, TripCount, "loop-predicate");
  
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

void TASForLoop::setLoopBody(BasicBlock * EntryBB, BasicBlock * ExitingBB) {
  EntryBody = EntryBB;
  ExitingBody = ExitingBB;
  Header->getTerminator()->setSuccessor(0, EntryBody);
  ExitingBody->getTerminator()->setSuccessor(0, Latch);
}

}
