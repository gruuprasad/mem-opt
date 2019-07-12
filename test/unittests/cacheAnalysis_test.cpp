#include "catch.hpp"
#include "CacheUsageAnalysis.h"
#include "Util.h"

#include <llvm/AsmParser/Parser.h>
#include <llvm/ADT/SmallVector.h>
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

#include <string>

extern std::string input_dir; // This contains path to input test files

using namespace llvm;

static std::unique_ptr<Module> parseIR(LLVMContext &C, const char *IR) {
  SMDiagnostic Err;
  std::unique_ptr<Module> Mod = parseAssemblyString(IR, Err, C);
  if (!Mod)
    Err.print("CacheAnalysisTests", errs());
  return Mod;
}

namespace tas {

TEST_CASE("Calculate struct size") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("cachecheck.ll"),  Err, C));
  REQUIRE( M != nullptr);
}

TEST_CASE("struct size test") {
  LLVMContext C;
  std::unique_ptr<Module> M = parseIR(C, R"(

        source_filename = "test.c"
        target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
        target triple = "x86_64-pc-linux-gnu"

        %struct.A = type <{ i32, i32, %struct.B, i16 }>
        %struct.B = type <{ i32, i16 }>

        define void @fnA() {
        %1 = alloca %struct.A, align 4
        %2 = alloca %struct.B, align 4
        %3 = getelementptr inbounds %struct.A, %struct.A* %1, i32 0, i32 0
        store i32 10, i32* %3, align 4
        %4 = getelementptr inbounds %struct.A, %struct.A* %1, i32 0, i32 1
        store i32 20, i32* %4, align 4
        %5 = getelementptr inbounds %struct.B, %struct.B* %2, i32 0, i32 0
        store i32 30, i32* %5, align 4
        %6 = getelementptr inbounds %struct.A, %struct.A* %1, i32 0, i32 2
        %7 = getelementptr inbounds %struct.B, %struct.B* %6, i32 0, i32 0
        store i32 40, i32* %7, align 4
        %8 = getelementptr inbounds %struct.A, %struct.A* %1, i32 0, i32 3
        store i16 50, i16* %8, align 4
        ret void
        }
  )");

  REQUIRE( M != nullptr);
  /*
  TypeFinder types;
  types.run(*M, true);

  auto & DL = M->getDataLayout();

  for (auto * Ty : types) {
    errs() << "Type name = " << Ty->getName() << "\n";
    auto * TyLayout = DL.getStructLayout(Ty);
    errs() << "struct size = " << TyLayout->getSizeInBytes() << ":" << TyLayout->getSizeInBits() << "\n";
    errs() << "Struct size (custom fn) " << getTypeSizeInBits(Ty) << "\n";
    errs() << "Member offset details\n";
    for (unsigned i = 0; i < Ty->getNumElements(); ++i) {
      errs() << TyLayout->getElementOffset(i) << ":" << TyLayout->getElementOffsetInBits(i) << "\n";
    }
  }
  */
}

TEST_CASE("Struct fit in single cache line", "[RUN]") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("cache_test1.ll"),  Err, C));
  REQUIRE( M != nullptr);
  auto F = M->getFunction("test_fn");

  CacheUsageAnalysis CA (F);
  CA.run();

  REQUIRE(CA.getNumOfCacheLines() == 1);
}

TEST_CASE("Struct fit across two cache line") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("cache_test2.ll"),  Err, C));
  REQUIRE( M != nullptr);
  auto F = M->getFunction("test_fn");

  CacheUsageAnalysis CA (F);
  CA.run();

  REQUIRE(CA.getNumOfCacheLines() == 3);
}

TEST_CASE("Nested struct [level 2]") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("cache_test3.ll"),  Err, C));
  REQUIRE( M != nullptr);
  auto F = M->getFunction("test_fn");

  CacheUsageAnalysis CA (F);
  CA.run();

  REQUIRE(CA.getNumOfCacheLines() == 6);
}

TEST_CASE("Nested struct [level 3]") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("cache_test4.ll"),  Err, C));
  REQUIRE( M != nullptr);
  auto F = M->getFunction("test_fn");

  CacheUsageAnalysis CA (F);
  CA.run();

  REQUIRE(CA.getNumOfCacheLines() == 7);
}

TEST_CASE("array of struct [level 3]") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("cache_test5.ll"),  Err, C));
  REQUIRE( M != nullptr);
  auto F = M->getFunction("test_fn");

  CacheUsageAnalysis CA (F);
  CA.run();

  REQUIRE(CA.getNumOfCacheLines() == 4);
}

TEST_CASE("List of pointers") {
  LLVMContext C;
  SMDiagnostic Err;
  
  std::unique_ptr<Module> M (parseIRFile(input_dir + std::string("cache_test6.ll"),  Err, C));
  REQUIRE( M != nullptr);
  auto F = M->getFunction("test_fn");

  CacheUsageAnalysis CA (F);
  CA.run();

  REQUIRE(CA.getNumOfCacheLines() == 4);
}

}
