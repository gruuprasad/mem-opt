#include "catch.hpp"

#include "BatchMaker.h"
#include "Util.h"
#include "ToolUtil.h"
#include "PacketPathAnalysis.h"
#include "BlockPredication.h"

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
