#include "catch.hpp"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include "Util.h"
#include <memory.h>

extern std::string input_dir; // This contains path to input test files

using namespace llvm;
using namespace std;
using namespace tas;

static LLVMContext C;
static SMDiagnostic Err; 

static unique_ptr<Module> parseIR(string Filename, string FileDir) {
  std::unique_ptr<Module> M (parseIRFile(FileDir + Filename,  Err, C));
  if (!M)
    Err.print("Error parsing IR: ", errs());
  return M;
}

TEST_CASE("detect TAS_MAKE_BATCH annotation") {
  auto M = parseIR(std::string("batchmaker_test1.ll"), input_dir);
  REQUIRE( M != nullptr);

  map<Function *, string> AnnotatedFnList;
  getAnnotatedFnList(M.get(), AnnotatedFnList);

  REQUIRE(AnnotatedFnList.size() == 2);
  auto Fn = AnnotatedFnList.begin();
  //REQUIRE(Fn->getFirst()->getName().compare(std::string("test_fn")) == 0);
  REQUIRE(Fn->second.compare(std::string("tas_batch_maker")) == 0);
  Fn++;
  //REQUIRE(Fn->getFirst()->getName().compare(std::string("test_fn2")) == 0);
  REQUIRE(Fn->second.compare(std::string("tas_batch_maker")) == 0);
}

TEST_CASE("detect Batching Parameters") {
  auto M = parseIR(std::string("batchmaker_test1.ll"), input_dir);
  REQUIRE( M != nullptr);

  {
    // Function with one batch argument
    auto F = M->getFunction("test_fn3");
    llvm::SmallPtrSet<llvm::Value *, 4> BatchArgs;
    detectBatchParameters(F, BatchArgs);
    REQUIRE(BatchArgs.size() == 1);
  }

  {
    // Function with two batch argument
    auto F = M->getFunction("test_fn4");
    llvm::SmallPtrSet<llvm::Value *, 4> BatchArgs;
    detectBatchParameters(F, BatchArgs);
    REQUIRE(BatchArgs.size() == 2);
  }

  {
    // Function with two batch argument separated by one non-batch argument
    auto F = M->getFunction("test_fn5");
    llvm::SmallPtrSet<llvm::Value *, 4> BatchArgs;
    detectBatchParameters(F, BatchArgs);
    REQUIRE(BatchArgs.size() == 2);
  }
}

TEST_CASE("detect expensive variables") {
  auto M = parseIR(std::string("batchmaker_test1.ll"), input_dir);
  REQUIRE( M != nullptr);

  {
    // Function with two expensive variables
    auto F = M->getFunction("test_fn6");
    auto ExpensiveVars = detectExpPtrVars(F);
    REQUIRE(ExpensiveVars.size() == 2);
  }
}
