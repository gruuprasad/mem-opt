#include "BatchMaker/BatchMaker.h"
#include "catch.hpp"
#include "Common/Util.h"
#include "Common/ToolUtil.h"
#include "PacketPathAnalysis/PacketPathAnalysis.h"
#include "BlockPredication/BlockPredication.h"

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

static std::unique_ptr<Module> parseIR(std::string Filename, std::string FileDir = "") {
  std::unique_ptr<Module> M (parseIRFile(FileDir + Filename,  Err, C));
  if (!M)
    Err.print("BatchMaker_tests:", errs());
  return M;
}

TEST_CASE("detect TAS_MAKE_BATCH annotation") {
  auto M = parseIR(std::string("batchmaker_test1.ll"), input_dir);
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
  auto M = parseIR(std::string("batchmaker_test1.ll"), input_dir);
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
  auto M = parseIR(std::string("batchmaker_test1.ll"), input_dir);
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
  auto M = parseIR(std::string("batchmaker_test2.ll"), input_dir);
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
TEST_CASE("Topological sorting of CFG") {
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

TEST_CASE("detect path traces", "[RUN]") {
  auto M = parseIR(generateIR(std::string("goto_test1.c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("main");
  PacketPathAnalysis PD(F);
  REQUIRE(PD.getNumerOfPaths() == 2);
  auto & PathList = PD.getPathSetRef();
  // Expected number of intermediate basic blocks in each path.
  REQUIRE( PathList[1].size() == 2);
  REQUIRE( PathList[2].size() == 1);
}

TEST_CASE("detect path traces mulltiple goto targets") {
  auto M = parseIR(generateIR(std::string("batchmaker_test2.c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("process_packet");
  PacketPathAnalysis PD(F);
  REQUIRE(PD.getNumerOfPaths() == 3);
  auto & PathList = PD.getPathSetRef();
  // Expected number of intermediate basic blocks in each path.
  REQUIRE( PathList[1].size() == 2);
  REQUIRE( PathList[2].size() == 5);
  REQUIRE( PathList[3].size() == 6);
}

TEST_CASE("predicated block execution, single goto") {
  auto M = parseIR(generateIR(std::string("ifelse_goto.c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("if_else_fn");
  BlockPredication BP(F);
  BP.run();
  // Verifed the transformation manually
  // XXX How to test this automagically?!!
}

TEST_CASE("predicated block execution, multiple goto") {
  auto M = parseIR(generateIR(std::string("ifelse_multi_goto.c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("process_packet");
  BlockPredication BP(F);
  BP.run();
  // TODO Verify the transformation manually
  // XXX How to test this automagically?!!
}
