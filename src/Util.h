#ifndef TAS_UTIL_H
#define TAS_UTIL_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/Transforms/Utils/ValueMapper.h>

#include <map>
#include <string>

namespace tas {

class TasException {
public:
  llvm::Function * F_state;
  TasException(llvm::Function * F_) : F_state(F_) {}
  void dump() { F_state->print(llvm::errs()); }
};

enum class AnnotationType {
  UNKNOWN,
  EXPENSIVE_PTR,
  BATCH_ARG
};

void getAnnotatedFnList(llvm::Module * M, std::map<llvm::Function *,
                              std::string> & FnList);

void setAnnotationInFunctionObject(llvm::Module * M);

void cloneLoopBasicBlocks(llvm::Function * F, llvm::Loop * L,
                          llvm::ValueToValueMapTy & VMap);

llvm::CallInst * insertLLVMPrefetchIntrinsic(llvm::Function * F, llvm::Instruction * I, llvm::Instruction * InsertBefore);

void replaceUsesWithinBB(llvm::Value * From, llvm::Value * To,
                         llvm::BasicBlock * BB);

unsigned getTypeSizeInBits(llvm::Type * Ty);

llvm::SmallVector<llvm::Value *, 4> detectExpPtrVars(llvm::Function * F);

llvm::SmallVector<llvm::LoadInst *, 4>
detectExpPtrUses(llvm::SmallVectorImpl<llvm::Value *> & AnnotatedVars);

void detectBatchParameters(llvm::Function * F,
                          llvm::SmallPtrSet<llvm::Value *, 4> & BatchParameters);

llvm::Instruction * findBatchBeginMarkerInstruction(llvm::Function * F);

llvm::Value * createArray(llvm::Function * F, llvm::Type * Ty, unsigned size);

template <typename InstType>
llvm::Instruction * findValueFirstUseInInstruction(llvm::Value * V);

unsigned getGEPIndex(const llvm::GetElementPtrInst * GEP);

llvm::StoreInst * findFirstUseInStoreInst(llvm::Value * V);

void setSuccessor(llvm::BasicBlock * BB, llvm::BasicBlock * SuccBB,
                  unsigned Idx = 0);

void cloneBasicBlocksInto(llvm::Function * From, llvm::Function * To);


void getReturnBlocks(llvm::Function * F,
                     llvm::SmallVectorImpl<llvm::BasicBlock *> & Returns);

void getReturnInstList(llvm::Function * F,
                       llvm::SmallVectorImpl<llvm::ReturnInst *> & Result);

std::string writeToBitCodeFile(llvm::Module & M);

std::string writeToAsmFile(llvm::Module & M);

llvm::Value * addIncrementIndexOp(llvm::AllocaInst * IdxPtr,
                                  llvm::Instruction * InsertBefore);

void printRegeionInfo(llvm::Function * F);

std::pair<llvm::BasicBlock *, llvm::BasicBlock *>
unifyFunctionExitNodes(llvm::Function & F);

template <typename InstType>
auto findLastUseOfValueInInstType(llvm::Value * V) {
  auto FU = find_if(V->users(),
            [&] (const auto * U) {
              return llvm::isa<InstType>(U)?true:false;
            });
  return llvm::cast<InstType>(*FU);
}

template <typename InstType>
auto findFirstUseOfValueInInstType(llvm::Value * V) {
  const InstType * LU = nullptr;
  for_each(V->users(),
          [&] (const auto * U) {
          if (llvm::isa<InstType>(U)) LU = llvm::cast<InstType>(U);
        });
  return LU;
}

llvm::AllocaInst * getLoopIndexVar(llvm::Loop * L);

const llvm::LoadInst * findEarliestPointerDerefInstruction(llvm::Value * V);

void visitSuccessor(llvm::SmallVectorImpl<llvm::BasicBlock *> & Blocks,
                    llvm::BasicBlock * CurBlock,
                    llvm::BasicBlock * EndBlock);

llvm::AllocaInst * getLoopTripCount(llvm::Loop * L0);

llvm::BasicBlock * getPreLoopBlock(llvm::Loop * L);

} // namespace tas

#endif
