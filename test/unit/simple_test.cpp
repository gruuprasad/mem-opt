#include "catch.hpp"
#include "Util.h"
#include "BatchProcess.h"

#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Support/SourceMgr.h"

#include <memory>

using namespace llvm;

namespace tas {

TEST_CASE("Read IR file") {

  LLVMContext Context;
  SMDiagnostic Err;
  std::unique_ptr<Module> M (parseIRFile("output.ll",  Err, Context));

  REQUIRE (M.get() != nullptr);
  auto * F = M->getFunction("fast_flows_packet_fss");
  
  auto BB = F->begin();
  ++BB;
  auto * TL0 = TASForLoop::Create(F->getContext(), &F->getEntryBlock(), &*BB, "tas.loop.0", F);
  F->print(errs());
}

}
