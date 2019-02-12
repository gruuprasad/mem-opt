#ifndef TAS_UTIL_H
#define TAS_UTIL_H

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

#define DEBUG_TYPE "tas"

namespace tas {

// Custom annotations added to the source code is stored as global value in LLVM IR
// under the identifer "llvm.global.annotations".
// This function adds the annotation identifier to the attribute list of Function object
// of in-memory IR. This facilitates applying function level transformation to the function
// of interest to us.
void setAnnotationInFunctionObject(llvm::Module * M);

// This function identifies pointer variables that are marked as expensive memory load
// operation. 
void detectVarAnnotation(llvm::Function * F);

} // namespace tas

#endif

