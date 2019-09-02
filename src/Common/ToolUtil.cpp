#include <iostream>
#include <string>

using namespace std;

namespace tas {

string generateIR(string InFile) {
  // Compile source to LLVM IR using clang
  string OutFile;
  if (!InFile.empty()) {
    OutFile = InFile.substr(0, InFile.find_last_of(".")) +  string(".ll");
    string CompileCmd = string("clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm -o ")
                        + OutFile + " " + InFile;

    auto ret = system(CompileCmd.c_str());
    if (ret != 0) {
      cerr << "Compilation failed with error code " << ret;
      exit(EXIT_FAILURE);
    }
  }
  return OutFile;
}

}
