#include "catch.hpp"

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Pass.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>

using namespace llvm;

TEST_CASE("Parse IR file") {
  const char * fname = "../examples/tests/test.ll";
  SMDiagnostic Err;
  LLVMContext Context;
  std::unique_ptr<Module> M(parseIRFile(fname, Err, Context));
  M->getInstructionCount();

  REQUIRE(M->getInstructionCount() == 4);
}
