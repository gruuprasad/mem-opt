#ifndef TOOL_UTIL_H
#define TOOL_UTIL_H

#include <string>

namespace tas {
/// Generates LLVM IR from c program.
std::string generateIR(std::string InFile);

}

#endif
