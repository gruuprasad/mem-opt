#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace tas {

string generateIR(string InFile, string Input_dir, bool isTas = false) {
  // Compile source to LLVM IR using clang
  string OutFile;
  if (!InFile.empty()) {
    OutFile = InFile.substr(0, InFile.find_last_of(".")) +  string(".ll");
    string tags ("-O0 -Xclang -disable-O0-optnone -S -emit-llvm ");
    if (isTas) {
      tags += "-std=gnu99 -Wall -Werror -I. -I/home/gp/mpi-sws/tas/include/ -march=native "
        "-fno-omit-frame-pointer -Wno-unused-variable -Wno-unused-function "
        "-Wno-address-of-packed-member -Wno-sometimes-uninitialized "
        "-I/usr/share/dpdk/x86_64-default-linuxapp-gcc/include "
        "-I/usr/share/dpdk/x86_64-default-linuxapp-gcc/include/dpdk "
        "-I/home/gp/mpi-sws/tas -I/home/gp/mpi-sws/tas/lib/tas/include "
        "-I/home/gp/mpi-sws/tas/lib/sockets/include "
        "-I/home/gp/mpi-sws/tas/tas/include " 
        "-I/home/gp/mpi-sws/tas/tas/fast "; 
    }

    string GenerateIRCmd = string("clang ") + tags + string("-o") +
                           Input_dir + OutFile + " " + Input_dir + InFile;
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

string linkObjects(vector<string> Files, string OutFile = "", string Output_dir = "") {
  const char * Cstr = std::getenv("CC");
  auto CC = Cstr == nullptr ? string("clang") : string(Cstr);
  if (OutFile.empty()) OutFile = "a.out";

  ostringstream Args;
  copy(Files.begin(), Files.end(), ostream_iterator<string>(Args, " "));

  string LinkerCmd = CC + string(" -no-pie ") + Args.str() + " -o " + Output_dir + OutFile;

  auto ret = system(LinkerCmd.c_str());
  if (ret != 0) {
    cerr << "clang:linker failed with error code " << ret << "\n";
    exit(EXIT_FAILURE);
  }
  return Output_dir + OutFile;
}

bool compileWithClangDriver(vector<string> Files, string Input_dir = "") {
  if (!Input_dir.empty())
    for_each(Files.begin(), Files.end(), [&] (string & File) { File.insert(0, Input_dir); });

  ostringstream Args;
  copy(Files.begin(), Files.end(), ostream_iterator<string>(Args, " "));

  string ClangCmd = string("clang -O0 -c ") + Args.str();
  auto ret = system(ClangCmd.c_str());
  if (ret != 0) {
    cerr << "clanLg:linker failed with error code " << ret << "\n";
    exit(EXIT_FAILURE);
  }
  return true;
}

}
