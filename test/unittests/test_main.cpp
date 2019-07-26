#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>

#include <string>

using namespace llvm;
std::string input_dir;

cl::opt<std::string> TestInputDir(cl::Positional, cl::Required, cl::desc("<test input dir>"));

int main(int argc, char* argv[]) {

   cl::ParseCommandLineOptions(argc, argv);
   input_dir = TestInputDir;
   Catch::Session session;
   int result = session.run();

  return result;
}
