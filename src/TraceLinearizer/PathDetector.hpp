#ifndef PATH_DETECTOR_HPP
#define PATH_DETECTOR_HPP

#include <llvm/IR/Function.h>

namespace tas {

class PathDetector {
  llvm::Function * F;

public:
  PathDetector(llvm::Function * F_) : F(F_) {}
  void DetectExitingBlocks();
};

}

#endif
