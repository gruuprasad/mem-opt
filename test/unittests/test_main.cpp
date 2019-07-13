#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <glog/logging.h>

#include <string>

std::string input_dir;

int main(int argc, char* argv[]) {

  google::InitGoogleLogging(argv[0]);
   Catch::Session session;

   using namespace Catch::clara;
   auto cli = session.cli()
     | Opt(input_dir, "input directory")
     ["--input_dir"]
     ("input test directory");

   session.cli(cli);

   int returnCode = session.applyCommandLine(argc, argv);
   if (returnCode != 0)
     return returnCode;

   if (input_dir.empty()) {
     std::cerr << "Error:input directory for test files not given --input_dir <dir_with_path>\n";
     return EXIT_FAILURE;
   }

   int result = session.run();

  return result;
}
