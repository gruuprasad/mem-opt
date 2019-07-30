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

#include "CacheUsageAnalysisPass.h"
#include "CacheUsageInfo.h"

#define DEBUG_TYPE "tasopt"

using namespace std;
using namespace llvm;

cl::opt<string> InFile(cl::Positional, cl::desc("<Module to analyze>"),
                       cl::value_desc("IR assembly filename"), cl::Required);

cl::opt<unsigned> CacheLineSize ("cacheline-size",
                cl::desc("cache line size in bytes (default - 64)"), cl::init(64));

static void analyseCache(Module * M, unsigned CacheLineSize) {
  legacy::PassManager FPM;
  FPM.add(new DominatorTreeWrapperPass());
  FPM.add(new LoopInfoWrapperPass());
  FPM.add(new tas::CacheUsageAnalysisPass(CacheLineSize));
  FPM.run(*M);

  /*
  FunctionAnalysisManager FAM (true);
  FAM.registerPass([&] { return tas::CacheUsageAnalysis(CacheLineSize); });
  FunctionPassManager FPM;

  ModuleAnalysisManager MAM (true);
  MAM.registerPass([&] { return FunctionAnalysisManagerModuleProxy(FAM); });

  ModulePassManager MPM;
  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
  errs() << "Running pass\n";
  MPM.run(*M, MAM);
  */
}

int main(int argc, char * argv[]) {
  cl::ParseCommandLineOptions(argc, argv);

  // Construct IR file from filename passed on the command line.
  LLVMContext Ctx;
  SMDiagnostic Err;
  unique_ptr<Module> M = parseIRFile(InFile, Err, Ctx);
  if (!M.get()) {
    errs() << "Error reading IR file.\n";
    Err.print(argv[0], errs());
    return -1;
  }

  analyseCache(M.get(), CacheLineSize);

  return 0;
}
