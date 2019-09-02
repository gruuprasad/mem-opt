#include "catch.hpp"
#include "Common/Util.h"

#include <llvm/AsmParser/Parser.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/IR/Verifier.h>         /* verifyFunction, verifyModule */
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SourceMgr.h>


extern std::string input_dir; // This contains path to input test files

using namespace llvm;
using namespace tas;

TEST_CASE("detect TAS_MAKE_BATCH annotation") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("batchmaker_test1.ll"),  Err, C));

  if (!M)
    Err.print("BatchMaker_tests:", errs());
  REQUIRE( M != nullptr);

  DenseMap<Function *, StringRef> AnnotatedFnList;
  getAnnotatedFunctionList(M.get(), AnnotatedFnList);

  REQUIRE(AnnotatedFnList.size() == 2);
  auto Fn = AnnotatedFnList.begin();
  REQUIRE(Fn->getFirst()->getName().compare(std::string("test_fn")) == 0);
  REQUIRE(Fn->getSecond().compare(std::string("tas_batch_maker")) == 0);
  Fn++;
  REQUIRE(Fn->getFirst()->getName().compare(std::string("test_fn2")) == 0);
  REQUIRE(Fn->getSecond().compare(std::string("tas_batch_maker")) == 0);
}
