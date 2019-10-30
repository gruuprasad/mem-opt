#include <llvm/Analysis/LoopInfo.h>
#include <llvm/AsmParser/Parser.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/LegacyPassManager.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/SourceMgr.h>

#include "BatchMaker.h"
#include "BlockPredication.h"
#include "LoopSplitter.h"
#include "ToolUtil.h"
#include "Util.h"

#include <cstdlib>
#include <map>
#include <string>

#define DEBUG_TYPE "tasopt"

using namespace std;
using namespace llvm;
using namespace tas;

cl::opt<string> SrcFile("c", cl::desc("c/c++ source file to analyze"),
                       cl::value_desc("filename"));

cl::opt<string> IRFile("a", cl::desc("IR assembly file to analyze"),
                       cl::value_desc("filename"));

cl::opt<unsigned> CacheLineSize ("cacheline-size",
                cl::desc("cache line size in bytes (default - 64)"), cl::init(64));

LLVMContext Ctx;
SMDiagnostic Err;

static unique_ptr<Module> parseIR(string Filename, string FileDir) {
  std::unique_ptr<Module> M (parseIRFile(FileDir + Filename,  Err, Ctx));
  if (!M)
    Err.print("Error parsing IR: ", errs());
  return M;
}

int main(int argc, char * argv[]) {
  cl::ParseCommandLineOptions(argc, argv);

  if (SrcFile.empty()) {
    errs() << "Source file is not given\n";
    Err.print(argv[0], errs());
    return -1;
  }
  auto OutFile = tas::generateIR(SrcFile, "", true);
  if (OutFile.empty()) {
    errs() << "Error reading source file.\n";
    Err.print(argv[0], errs());
    return -1;
  }
  auto M = parseIR(OutFile, "");

  if (!M) {
    errs() << "Error reading IR file.\n";
    Err.print(argv[0], errs());
    return -1;
  }

  map<Function *, string> FnLists;
  tas::getAnnotatedFnList(M.get(), FnLists);

  errs() << "number of annotated functions = " << FnLists.size() << "\n";

  for (auto & FnStr : FnLists) {
    if (FnStr.second.compare("tas_block_predicate") != 0) continue;
    // Block Predication
    tas::BlockPredication BP(FnStr.first);
    auto res = BP.run();
    if (!res) {
      errs() << "CFG linearization failed for fn: " << FnStr.second << "\n";
      Err.print(argv[0], errs());
      return -1;
    }

    // Make Batch version
    tas::BatchMaker BM(FnStr.first);
    auto BatchFunc = BM.run();

    writeToAsmFile(*M);
    // Loop Splitting
    DominatorTree DT(*BatchFunc);
    LoopInfo LI(DT);
    tas::LoopSplitter LS(BatchFunc, &LI);
    res = LS.run();
    errs() << res << "\n";
  }

  writeToAsmFile(*M);
  auto TestObject = generateObject(writeToBitCodeFile(*M));
  return 0;
}
