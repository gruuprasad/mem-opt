#include "catch.hpp"

#include "LoopSplitter.h"
#include "Util.h"
#include "ToolUtil.h"

#include <llvm/AsmParser/Parser.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/ADT/SCCIterator.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/IR/Verifier.h>
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

TEST_CASE("fn with single loop") {
  std::string filePrefix = "loopsplitter_test1";
  std::string functionName = "fn";

  auto IR = generateIR(filePrefix + string(".c"), input_dir);

  /*
  vector<string> OptList {"indvars", "loop-simplify", "lcssa"};
  IR= runOpt(IR, input_dir, OptList);
  */

  auto M = parseIR(IR, input_dir);
  REQUIRE(M != nullptr);

  // Function with 2 arguments and int return type.
  auto F = M->getFunction(functionName);

  DominatorTree DT(*F);
  LoopInfo LI(DT);

  LoopSplitter LS(F, &LI);
  LS.run();

  auto Stats = LS.getStats();
  REQUIRE(Stats.AnnotatedVarsSize == 1);
  REQUIRE(Stats.VarUsePointsSize == 1);

  //F->print(errs());
  auto asmFile = writeToAsmFile(*M);
  auto TestObject = generateObject(asmFile);

  auto binary = linkObjects(vector<string>{TestObject}, string("loopSplitter_test11"));

  // Run the binary
  binary.insert(0, "./");
  auto ret = system(binary.c_str());
  REQUIRE(ret == 0);
}
