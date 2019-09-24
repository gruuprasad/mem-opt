#ifndef TOOL_UTIL_H
#define TOOL_UTIL_H

#include <string>
#include <vector>

namespace tas {
/// Generates LLVM IR from c program.
std::string generateIR(std::string InFile, std::string Input_dir);

std::string generateObject(std::string InFile, std::string Input_dir = "");

bool compileBinary(std::vector<std::string> Files, std::string OutFile = "",
                  std::string Output_dir = "");

bool compileWithClangDriver(std::vector<std::string> Files, std::string Input_dir = "");
}

#endif