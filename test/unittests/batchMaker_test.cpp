#include "BatchMaker/BatchMaker.h"
#include "catch.hpp"
#include "Common/Util.h"

#include <llvm/AsmParser/Parser.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/ADT/SCCIterator.h>
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

TEST_CASE("create batch function prototype") {
  auto M = parseIR(std::string("batchmaker_test2.ll"));
  REQUIRE( M != nullptr);

  {
    // Test whether new function prototype exists in the module
    auto F = M->getFunction("process_packet");
    BatchMaker BM(F);
    BM.run();

    auto NewF = M->getFunction("process_packet_batch");
    REQUIRE(NewF != nullptr);
  }
}

/* Explore graph related tools available in LLVM.
TEST_CASE("Reverse Topological sorting of CFG") {
  auto M = parseIR(std::string("batchmaker_test2.ll"));
  REQUIRE( M != nullptr);
  auto F = M->getFunction("process_packet");
  errs() << "========Reverse post order=====\n";
  ReversePostOrderTraversal<Function*> RPOT(F);
  for (auto I = RPOT.begin(); I != RPOT.end(); ++I) {
    errs() << **I;
  }

  errs() << "========SCC post order=====\n";
  for (auto I = scc_begin(F); I != scc_end(F); ++I) {
    const auto & SCCBBs = *I;
    errs() << "SCC\n";
    for (auto & BI : SCCBBs) {
      errs() << *BI;
    }
  }

  errs() << "======Post order========\n";
  for (auto I = po_begin(F); I != po_end(F); ++I) {
    errs() << **I;
  }
}
*/
