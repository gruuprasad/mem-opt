#include "catch.hpp"
#include "ToolUtil.h"

#include <string>
#include <vector>

extern std::string input_dir; // This contains path to input test files

using namespace tas;
using namespace std;

TEST_CASE("generate IR") {
  auto IRFile = generateIR(std::string("hello.c"), input_dir);
  REQUIRE(IRFile == string("hello.ll"));
}

TEST_CASE("generate object") {
  auto ObjectFile = generateObject(std::string("hello.ll"), input_dir);
  REQUIRE(ObjectFile == input_dir + string("hello.o"));
}

TEST_CASE("compile binary") {
  auto IRFile = generateIR(std::string("hello.c"), input_dir);
  auto HelloObject = generateObject(std::string("hello.ll"), input_dir);
  auto Main = generateObject(std::string("main.c"), input_dir);
  auto Binary = compileBinary(vector<string>{HelloObject, Main}, string("hello"));

  REQUIRE(Binary == true);
}

TEST_CASE("compile with clang driver") {
  vector<string> files = { "hello.c", "main.c" };
  auto Ret = compileWithClangDriver(files, input_dir);
  REQUIRE( Ret == true);
}
