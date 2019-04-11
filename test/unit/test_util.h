#ifndef TAS_TEST_UTIL_H
#define TAS_TEST_UTIL_H

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

#include <iostream>
#include <memory>

namespace tas {

std::unique_ptr<llvm::Module> readIRFile(const char * fname) {
  llvm::SMDiagnostic Err;
  llvm::LLVMContext Context;
  auto M = parseIRFile(fname, Err, Context);
  return std::move(M);
}

}

#endif
