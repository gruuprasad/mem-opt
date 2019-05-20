#include "catch.hpp"
#include "test_util.h"
#include "Util.h"

#include <llvm/AsmParser/Parser.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Support/SourceMgr.h"

#include <iostream>
#include <memory>

using namespace llvm;

/*
TEST_CASE("Parse IR file") {
  auto M = tas::readIRFile("../examples/tests/test.ll");
  //REQUIRE(M->getInstructionCount() == 4);
}
*/

static std::unique_ptr<Module> makeLLVMModule(LLVMContext &Context,
                                              const char *ModuleStr) {
  SMDiagnostic Err;
  return parseAssemblyString(ModuleStr, Err, Context);
}

TEST_CASE("Detect annotated variables") {

  const char * ModuleStr = 
    "target datalayout = \"e-m:e-i64:64-f80:128-n8:16:32:64-S128\"\n"
    "%struct.packet = type { i32, i64 }\n"
    "define void @hash_lookup(%struct.packet* pkt) {\n"
    "entry:\n"
    "%pkt.addr = alloca %struct.packet*, align 8\n"
    "%h = alloca i32, align 4\n"
    "%p = alloca %struct.packet*, align 8\n"
    "store %struct.packet* %pkt, %struct.packet** %pkt.addr, align 8\n"
    "%pkt.addr1 = bitcast %struct.packet** %pkt.addr to i8*\n"
    "call void @llvm.var.annotation(i8* %pkt.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 73)\n"
    "%p2 = bitcast %struct.packet** %p to i8*\n"
    "call void @llvm.var.annotation(i8* %p2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 75)\n"
    "ret void\n"
    "}\n"
    "declare void @llvm.var.annotation(i8*, i8*, i8*, i32)\n";

  LLVMContext Context;
  std::unique_ptr<Module> M = makeLLVMModule(Context, ModuleStr);

  REQUIRE (M.get() != nullptr);

  Function &F = *M->begin();

  SmallVector<Value *, 8> AV;
  //tas::detectPrefetchVariables(&F, AV);
  REQUIRE (AV.size() == 2);
}
