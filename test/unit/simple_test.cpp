#include "catch.hpp"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>         /* verifyFunction, verifyModule */
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SourceMgr.h>

#include <string>

extern std::string input_dir;

using namespace llvm;

namespace tas {

TEST_CASE("detect annotated variables") {
  LLVMContext Context;
  SMDiagnostic Err;
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("detect_annotated_vars.ll"),  Err, Context));
  REQUIRE (M.get() != nullptr);
  auto * F = M->getFunction("test_fn");

  SmallVector<Value *, 4> EP;  // Expensive pointers
  SmallVector<Value *, 4> BP; // Batch parameters
  detectAnnotatedVariable(F, EP, BP);
  REQUIRE(EP.size() == 1);
  REQUIRE(BP.size() == 2);
}

}
