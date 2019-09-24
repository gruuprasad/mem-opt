#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace tas {

string generateIR(string InFile, string Input_dir = "") {
  // Compile source to LLVM IR using clang
  string OutFile;
  if (!InFile.empty()) {
    OutFile = InFile.substr(0, InFile.find_last_of(".")) +  string(".ll");
    string GenerateIRCmd = string("clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm -o ")
                        + Input_dir + OutFile + " " + Input_dir + InFile;

    auto ret = system(GenerateIRCmd.c_str());
    if (ret != 0) {
      cerr << "clang:IR generation failed with error code " << ret << "\n";
      exit(EXIT_FAILURE);
    }
  }
  return OutFile;
}

string generateObject(string InFile, string Input_dir = "") {
  if (InFile.empty()) return string();

  auto Idx = InFile.find_last_of(".");
  auto FileName = InFile.substr(0, Idx);
  auto FileType = InFile.substr(Idx + 1);

  if (FileType == string("c"))
    InFile = generateIR(InFile, Input_dir);

  // Create Object file.
  auto ObjectFile =  FileName + string(".o");
  string CompileCmd = string("llc -filetype=obj ") + Input_dir + InFile +
                             " -o " + Input_dir + ObjectFile;
  auto ret = system(CompileCmd.c_str());
  if (ret != 0) {
    cerr << "llc:Compilation failed with error code " << ret << "\n";
    exit(EXIT_FAILURE);
  }

  return Input_dir + ObjectFile;
}

bool compileBinary(vector<string> Files, string OutFile = "", string Output_dir = "") {
  if (OutFile.empty()) OutFile = "a.out";

  ostringstream Args;
  copy(Files.begin(), Files.end(), ostream_iterator<string>(Args, " "));

  string LinkerCmd = string("clang ") + Args.str() + " -o " + Output_dir + OutFile;
  cout << LinkerCmd << "\n";

  auto ret = system(LinkerCmd.c_str());
  if (ret != 0) {
    cerr << "clang:linker failed with error code " << ret << "\n";
    exit(EXIT_FAILURE);
  }
  return true;
}

void compileWithClangDriver(vector<string> Files, string Input_dir = "") {
  string OptLevel = " -O2 ";

  ostringstream Args;
  copy(Files.begin(), Files.end(), ostream_iterator<string>(Args, " "));

  string ClangCmd = string("clang ") + OptLevel + string("-fPIC -c ") + Args.str();
  auto ret = system(ClangCmd.c_str());
  if (ret != 0) {
    cerr << "clanLg:linker failed with error code " << ret << "\n";
    exit(EXIT_FAILURE);
  }
}

}
