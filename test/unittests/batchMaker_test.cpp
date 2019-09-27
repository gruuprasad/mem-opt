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
#include <llvm/Bitcode/BitcodeWriter.h>
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

TEST_CASE("simple batch case") {
  std::string filePrefix = "batchmaker_test3";
  std::string functionName = "simple_fn";

  auto M = parseIR(generateIR(filePrefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);

  // Function with 2 arguments and int return type.
  auto F = M->getFunction(functionName);
  REQUIRE(F->getReturnType() == Type::getInt32Ty(C));
  REQUIRE(F->arg_size() == 2);
  REQUIRE(M->getFunction(functionName + string("_batch")) == nullptr);

  BatchMaker BM(F);
  BM.run();

  // Function with 4 arguments and void return type.
  auto BF = M->getFunction(functionName + string("_batch"));
  REQUIRE(BF != nullptr);
  REQUIRE(BF->getReturnType() == Type::getVoidTy(C));
  REQUIRE(BF->arg_size() == 4);

  M->setSourceFileName(filePrefix + string("_batch.ll"));

  writeToAsmFile(*M);

  // MainObject contains checks to verify the correctness of transformation.
  auto MainObject = generateObject(filePrefix + string("_main.c"), input_dir);
  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));

  auto binary = linkObjects(vector<string>{TestObject, MainObject}, filePrefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);

}

TEST_CASE("batch multiple basic blocks") {
  std::string filePrefix = "batchmaker_test4";
  std::string functionName = "multiblock_fn";

  auto M = parseIR(generateIR(filePrefix + string(".c"), input_dir), input_dir);
  REQUIRE(M != nullptr);

  // Function with 2 arguments and int return type.
  auto F = M->getFunction(functionName);
  REQUIRE(F->getReturnType() == Type::getInt32Ty(C));
  REQUIRE(F->arg_size() == 2);
  REQUIRE(M->getFunction(functionName + string("_batch")) == nullptr);

  BatchMaker BM(F);
  BM.run();

  // Function with 4 arguments and void return type.
  auto BF = M->getFunction(functionName + string("_batch"));
  REQUIRE(BF != nullptr);
  REQUIRE(BF->getReturnType() == Type::getVoidTy(C));
  REQUIRE(BF->arg_size() == 4);

  M->setSourceFileName(filePrefix + string("_batch.ll"));

  writeToAsmFile(*M);

  // MainObject contains checks to verify the correctness of transformation.
  auto MainObject = generateObject(filePrefix + string("_main.c"), input_dir);
  // Generate object for unit under test.
  auto TestObject = generateObject(writeToBitCodeFile(*M));

  auto binary = linkObjects(vector<string>{TestObject, MainObject}, filePrefix);

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}
