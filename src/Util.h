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

void insertLLVMPrefetchIntrinsic(llvm::Function * F, llvm::Instruction * I,
                                 llvm::Value * PtrVal);

void replaceUsesWithinBB(llvm::Value * From, llvm::Value * To,
                         llvm::BasicBlock * BB);

unsigned getTypeSizeInBits(llvm::Type * Ty);

void detectExpPtrVars(llvm::Function * F,
                      llvm::SmallVectorImpl<llvm::Value *> & ExpensivePointers);

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

void getReturnInstList(llvm::Function * F,
                       llvm::SmallVectorImpl<llvm::ReturnInst *> & Result);

std::string writeToBitCodeFile(llvm::Module & M);

std::string writeToAsmFile(llvm::Module & M);

llvm::Value * addIncrementIndexOp(llvm::AllocaInst * IdxPtr,
                                  llvm::Instruction * InsertBefore);

void printRegeionInfo(llvm::Function * F);

llvm::BasicBlock * unifyFunctionExitNodes(llvm::Function & F);
} // namespace tas

#endif
