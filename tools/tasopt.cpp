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

#include "BlockPredication.h"
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

int main(int argc, char * argv[]) {
  cl::ParseCommandLineOptions(argc, argv);

  Module * M = nullptr;
  if (!SrcFile.empty()) {
    auto OutFile = tas::generateIR(SrcFile);
    if (OutFile.empty()) {
      errs() << "Error reading source file.\n";
      Err.print(argv[0], errs());
      return -1;
    }
    M = parseIRFile(OutFile, Err, Ctx).get();

    if (!M) {
      errs() << "Error reading IR file.\n";
      Err.print(argv[0], errs());
      return -1;
    }
  } else {
    errs() << "Source file is not given\n";
    Err.print(argv[0], errs());
    return -1;
  }

  map<Function *, string> FnLists;
  tas::getAnnotatedFnList(M, FnLists);

  for (auto & FnStr : FnLists) {
    if (FnStr.second.compare("tas_batch_maker") != 0) continue;
    tas::BlockPredication BP(FnStr.first);
    auto res = BP.run();
    if (!res) {
      errs() << "CFG linearization failed for fn: " << FnStr.second << "\n";
      Err.print(argv[0], errs());
      return -1;
    }
  }

  return 0;
}
