#include "catch.hpp"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include "BlockPredication.h"
#include "PacketPathAnalysis.h"
#include "ToolUtil.h"
#include "Util.h"
#include <memory.h>

extern std::string input_dir; // This contains path to input test files

using namespace llvm;
using namespace tas;

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
