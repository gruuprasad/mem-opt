#ifndef TAS_UTIL_H
#define TAS_UTIL_H

#include <llvm/IR/Module.h>

namespace tas {

// Custom annotations added to the source code is stored as global value in LLVM IR
// under the identifer "llvm.global.annotations".
// This function adds the annotation identifier to the attribute list of Function object
// of in-memory IR. This facilitates applying function level transformation to the function
// of interest to us.
void setAnnotationInFunctionObject(llvm::Module * M);

} // namespace tas

#endif

