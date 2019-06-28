#ifndef TAS_UTIL_H
#define TAS_UTIL_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/Transforms/Utils/ValueMapper.h>

namespace tas {

enum class AnnotationType {
  UNKNOWN,
  EXPENSIVE_PTR,
  BATCH_ARG
};

void getAnnotatedFunctionList(llvm::Module * M, llvm::DenseMap<llvm::Function * , llvm::StringRef> & FnList);

// Custom annotations added to the source code is stored as global value in LLVM IR
// under the identifer "llvm.global.annotations".
// This function adds the annotation identifier to the attribute list of Function object
// of in-memory IR. This facilitates applying function level transformation to the function
// of interest to us.
//
// Annotation list structure in IR: stored as array of struct
// ConstantArray : [Size x struct]
// struct definition: [Function  ptr, GlobalVariable ptr, GlobalVariable ptr, i32]
// @llvm.global.annotations = [N x {i8*, i8*, i8*, i32}]
// N - number of global annotations in a module
// Struct members details:
// i8* - Function pointer
// i8* - Pointer to annotation string
// i8* - Pointer to file name string
// i32 - line number of annotation in source file
void setAnnotationInFunctionObject(llvm::Module * M);

void cloneLoopBasicBlocks(llvm::Function * F, llvm::Loop * L, llvm::ValueToValueMapTy & VMap);

void insertLLVMPrefetchIntrinsic(llvm::Function * F, llvm::Instruction * I, llvm::Value * PtrVal);

void replaceUsesWithinBB(llvm::Value * From, llvm::Value * To, llvm::BasicBlock * BB);

unsigned getTypeSizeInBits(llvm::Type * Ty);

void detectExpensivePointerVariables(llvm::Function * F, llvm::SmallVector<llvm::Value *, 4> & ExpensivePointers);

void detectBatchingParameters(llvm::Function * F, llvm::SmallPtrSet<llvm::Value *, 4> & BatchParameters);

llvm::Value * createArray(llvm::Function * F, llvm::Type * Ty, unsigned size);
} // namespace tas

#endif

