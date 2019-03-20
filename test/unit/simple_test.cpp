#include "catch.hpp"
#include "test_util.h"

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>

using namespace llvm;

TEST_CASE("Parse IR file") {
  auto M = tas::readIRFile("../examples/tests/test.ll");
  REQUIRE(M->getInstructionCount() == 4);
}
