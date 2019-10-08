#include "catch.hpp"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Analysis/DominanceFrontier.h>
#include <llvm/Analysis/PostDominators.h>
#include <llvm/Analysis/RegionInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/CFG.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include "BlockPredication.h"
#include "PacketPathAnalysis.h"
#include "ToolUtil.h"
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

TEST_CASE("simple if else") {
  std::string fileprefix = "blockpredication_test1";
  auto M = parseIR(generateIR(fileprefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("fn");
  BlockPredication BP(F);
  BP.run();

  // For debug purpose
  auto asmFile = writeToAsmFile(*M);

  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));
  auto binary = linkObjects(vector<string>{TestObject}, fileprefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}

TEST_CASE("multiiple if else blocks") {
  std::string fileprefix = "blockpredication_test2";
  auto M = parseIR(generateIR(fileprefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("fn");
  BlockPredication BP(F);
  BP.run();

  // For debug purpose
  auto asmFile = writeToAsmFile(*M);

  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));
  auto binary = linkObjects(vector<string>{TestObject}, fileprefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}

TEST_CASE("single goto") {
  std::string fileprefix = "blockpredication_test3";
  auto M = parseIR(generateIR(fileprefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("fn");
  BlockPredication BP(F);
  BP.run();

  auto asmFile = writeToAsmFile(*M);

  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));

  auto binary = linkObjects(vector<string>{TestObject}, fileprefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}

TEST_CASE("predicated block execution, multiple goto") {
  std::string fileprefix = "blockpredication_test4";
  auto M = parseIR(generateIR(fileprefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("fn");

  BlockPredication BP(F);
  BP.run();

  auto asmFile = writeToAsmFile(*M);

  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));

  auto binary = linkObjects(vector<string>{TestObject}, fileprefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}

TEST_CASE("predicated block execution, multiple ifelse") {
  std::string fileprefix = "blockpredication_test5";
  auto M = parseIR(generateIR(fileprefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("multi_conditionals_fn");

  BlockPredication BP(F);
  BP.run();

  auto asmFile = writeToAsmFile(*M);

  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));

  auto binary = linkObjects(vector<string>{TestObject}, fileprefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}

TEST_CASE("lot of if else") {
  std::string fileprefix = "blockpredication_test6";
  auto M = parseIR(generateIR(fileprefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("fn");

  BlockPredication BP(F);
  BP.run();

  auto asmFile = writeToAsmFile(*M);

  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));

  auto binary = linkObjects(vector<string>{TestObject}, fileprefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}

TEST_CASE("fast_flows_packet") {
  std::string fileprefix = "fast_flows";
  auto M = parseIR(generateIR(fileprefix + string(".c"), input_dir, true), input_dir);
  REQUIRE(M != nullptr);
  auto F = M->getFunction("fast_flows_packet");

  errs() << "Running block predication\n";
  BlockPredication BP(F);
  BP.run();

  //auto asmFile = writeToAsmFile(*M);

  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));

  //auto binary = linkObjects(vector<string>{TestObject}, fileprefix);

  // Run the binary
  //binary.insert(0, "./");
  //auto ret = system(binary.c_str());
  //REQUIRE(ret == 0);
}
