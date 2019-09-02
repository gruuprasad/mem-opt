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

LLVMContext C;
SMDiagnostic Err;

static std::unique_ptr<Module> parseIR(std::string Filename) {
  std::unique_ptr<Module> M (parseIRFile(input_dir + Filename,  Err, C));
  if (!M)
    Err.print("BatchMaker_tests:", errs());
  return M;
}

TEST_CASE("detect TAS_MAKE_BATCH annotation") {
  auto M = parseIR(std::string("batchmaker_test1.ll"));
  REQUIRE( M != nullptr);

  DenseMap<Function *, StringRef> AnnotatedFnList;
  getAnnotatedFunctionList(M.get(), AnnotatedFnList);

  REQUIRE(AnnotatedFnList.size() == 2);
  auto Fn = AnnotatedFnList.begin();
  //REQUIRE(Fn->getFirst()->getName().compare(std::string("test_fn")) == 0);
  REQUIRE(Fn->getSecond().compare(std::string("tas_batch_maker")) == 0);
  Fn++;
  //REQUIRE(Fn->getFirst()->getName().compare(std::string("test_fn2")) == 0);
  REQUIRE(Fn->getSecond().compare(std::string("tas_batch_maker")) == 0);
}

TEST_CASE("detect Batching Parameters") {
  auto M = parseIR(std::string("batchmaker_test1.ll"));
  REQUIRE( M != nullptr);

  {
    // Function with one batch argument
    auto F = M->getFunction("test_fn3");
    llvm::SmallPtrSet<llvm::Value *, 4> BatchArgs;
    detectBatchingParameters(F, BatchArgs);
    REQUIRE(BatchArgs.size() == 1);
  }

  {
    // Function with two batch argument
    auto F = M->getFunction("test_fn4");
    llvm::SmallPtrSet<llvm::Value *, 4> BatchArgs;
    detectBatchingParameters(F, BatchArgs);
    REQUIRE(BatchArgs.size() == 2);
  }

  {
    // Function with two batch argument separated by one non-batch argument
    auto F = M->getFunction("test_fn5");
    llvm::SmallPtrSet<llvm::Value *, 4> BatchArgs;
    detectBatchingParameters(F, BatchArgs);
    REQUIRE(BatchArgs.size() == 2);
  }
}

TEST_CASE("detect expensive variables") {
  auto M = parseIR(std::string("batchmaker_test1.ll"));
  REQUIRE( M != nullptr);

  {
    // Function with two expensive variables
    auto F = M->getFunction("test_fn6");
    SmallVector<Value *, 4> ExpensiveVars;
    detectExpensivePointerVariables(F, ExpensiveVars);
    REQUIRE(ExpensiveVars.size() == 2);
  }
}
