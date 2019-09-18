#include <iostream>
#include <string>

using namespace std;

namespace tas {

string generateIR(string InFile, string Input_dir = "") {
  // Compile source to LLVM IR using clang
  string OutFile;
  if (!InFile.empty()) {
    OutFile = InFile.substr(0, InFile.find_last_of(".")) +  string(".ll");
    string CompileCmd = string("clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm -o ")
                        + OutFile + " " + Input_dir + InFile;

    auto ret = system(CompileCmd.c_str());
    if (ret != 0) {
      cerr << "Compilation failed with error code " << ret;
      exit(EXIT_FAILURE);
    }
  }
  return OutFile;
}

}
