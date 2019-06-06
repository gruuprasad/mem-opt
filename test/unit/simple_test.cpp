#include "catch.hpp"
#include "Util.h"

#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/Verifier.h"              /* verifyFunction, verifyModule */
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Support/SourceMgr.h"

#include <memory>

using namespace llvm;

namespace tas {

TEST_CASE("Read IR file") {
  LLVMContext Context;
  SMDiagnostic Err;
  std::unique_ptr<Module> M (parseIRFile("array_create.ll",  Err, Context));

  REQUIRE (M.get() != nullptr);
  auto * F = M->getFunction("main");
  errs() << F->getName() << ":\n";
  

  IRBuilder<> Builder (Context);
  auto BB = F->begin();
  auto TermI = (*BB).getTerminator();

  Builder.SetInsertPoint(TermI);
  auto arrayPtr = Builder.CreateAlloca(ArrayType::get(Builder.getInt32Ty(), 32));

  ++BB; ++BB;
  TermI = (*BB).getTerminator();
  //auto zero = ConstantInt::get(Context, APInt(64, 0, true));

  Builder.SetInsertPoint(TermI);
  auto index = ConstantInt::get(Context, APInt(32, 0, true));
  auto ptr = Builder.CreateGEP(arrayPtr, {Builder.getInt64(0), index}, "index");
  auto store = Builder.CreateStore(index, ptr);
  auto load = Builder.CreateLoad(ptr);

  F->print(errs());

  verifyModule(*M);
  M->dump();
}

}
