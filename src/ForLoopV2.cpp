#include "ForLoopV2.h"
#include "Util.h"

#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace tas {

void IRLoop::extractLoopSkeleton(Loop * L) {
  Header = L->getHeader();
  Latch = L->getLoopLatch();
  IdxAlloca = getLoopIndexVar(L);
  PreHeader = L->getLoopPreheader();
}

void IRLoop::analyze(Loop * L) {
  extractLoopSkeleton(L);
  
  // Copy loop body
  for (auto & BB : L->getBlocks()) {
    Blocks.push_back(BB);
  }
}

void IRLoop::constructEmptyLoop(Value * TripCount, BasicBlock * ExitBlock) {
  auto & Ctx = ExitBlock->getContext();
  const auto & F = ExitBlock->getParent();

  Latch = BasicBlock::Create(Ctx, "latch", F);
  Header = BasicBlock::Create(Ctx, "header", F, Latch);
  PreHeader = BasicBlock::Create(Ctx, "preheader", F, Header);
  EmptyBody = BasicBlock::Create(Ctx, "emptyblock", F, Latch);

  // Create Loop Index Variable.
  IRBuilder<> Builder(Ctx);
  Builder.SetInsertPoint(&F->getEntryBlock().front());
  IdxAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  // Set Index variable to 0 in preheader block.
  Builder.SetInsertPoint(PreHeader);
  Builder.CreateStore(Builder.getInt32(0), IdxAlloca);
  Builder.CreateBr(Header);

  // Populate latch block
  Builder.SetInsertPoint(Latch);
  auto BackEdge = Builder.CreateBr(Header);
  addIncrementIndexOp(IdxAlloca, BackEdge);

  // Branch Empty block to latch
  Builder.SetInsertPoint(EmptyBody);
  Builder.CreateBr(Latch);

  // Populate header block
  Builder.SetInsertPoint(Header);
  auto IdxVal = Builder.CreateLoad(IdxAlloca);
  auto * Icmp = Builder.CreateICmpSLT(IdxVal, TripCount, "loop-predicate");
  Builder.CreateCondBr(Icmp, EmptyBody, ExitBlock);
}

void IRLoop::setLoopBlocks(SmallVectorImpl<BasicBlock *> & Blocks) {
  for_each(Blocks, [&] (auto & BB) { Blocks.push_back(BB); });
  assert (!Blocks.empty() && "Blocks can't be empty");
  setSuccessor(Header, Blocks.front()); // True Path
  assert (Latch && "Latch is NULL");
  setSuccessor(Blocks.back(), Latch);
}

void LoopBodyTraverser::traverse(SmallVectorImpl<BasicBlock *> & Blocks,
                                 BasicBlock * Start, BasicBlock * End) {
  if (Start == End) return;
  Blocks.push_back(Start);
  for (auto * BB : successors(Start)){
    // Don't travel out of loop
    //if (std::find(ExitBlocks.begin(), ExitBlocks.end(), BB) != Blocks.end()) continue;
    traverse(Blocks, BB, End);
  }
}

}
