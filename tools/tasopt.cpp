//#include <llvm/Analysis/LoopInfo.h>
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

#include "PassWrapper/BatchMakerPass.h"
#include "PassWrapper/CacheUsageAnalysisPass.h"
#include "CacheUsageInfo.h"
#include "ToolUtil.h"

#include <cstdlib>

#define DEBUG_TYPE "tasopt"

using namespace std;
using namespace llvm;

cl::opt<string> SrcFile("c", cl::desc("c/c++ source file to analyze"),
                       cl::value_desc("filename"));

cl::opt<string> IRFile("a", cl::desc("IR assembly file to analyze"),
                       cl::value_desc("filename"));

cl::opt<unsigned> CacheLineSize ("cacheline-size",
                cl::desc("cache line size in bytes (default - 64)"), cl::init(64));

int main(int argc, char * argv[]) {
  cl::ParseCommandLineOptions(argc, argv);

  LLVMContext Ctx;
  SMDiagnostic Err;
  Module * M = nullptr;
  if (!SrcFile.empty()) {
    auto OutFile = tas::generateIR(SrcFile, std::string());
    // FIXME Reading IR file generated is failing.
    // M = parseIRFile(Outfile, Err, Ctx).get();
  }

  // Create LLVM Module from IR file.
  if (SrcFile.empty() && IRFile.empty()) {
    errs() << "Input file not given.\n";
    return EXIT_FAILURE;
  } else {
    M = parseIRFile(IRFile, Err, Ctx).get();
  }

  if (!M) {
    errs() << "Error reading IR file.\n";
    Err.print(argv[0], errs());
    return -1;
  }

  tas::runBatchMakerPass(M);

  return 0;
}
