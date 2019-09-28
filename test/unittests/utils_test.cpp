#include "catch.hpp"
#include "ToolUtil.h"

#include <string>
#include <vector>

extern std::string input_dir; // This contains path to input test files

using namespace tas;
using namespace std;

TEST_CASE("generate IR") {
  auto IRFile = generateIR(std::string("util_test1.c"), input_dir);
  REQUIRE(IRFile == string("util_test1.ll"));
}

TEST_CASE("generate object") {
  auto ObjectFile = generateObject(std::string("util_test1.ll"), input_dir);
  REQUIRE(ObjectFile == input_dir + string("util_test1.o"));
}

TEST_CASE("compile binary") {
  auto IRFile = generateIR(std::string("util_test1.c"), input_dir);
  auto HelloObject = generateObject(std::string("util_test1.ll"), input_dir);
  auto Binary = linkObjects(vector<string>{HelloObject}, string("util_test1"));

  REQUIRE(Binary == string("util_test1"));
}

TEST_CASE("compile with clang driver") {
  vector<string> files = { "util_test1.c"};
  auto Ret = compileWithClangDriver(files, input_dir);
  REQUIRE( Ret == true);
}
