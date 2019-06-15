#include "BatchMaker.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-maker"

namespace tas {

bool BatchMaker::run() {

  
  makeFnPrototypeBatchedForm();

  return false;
}

void BatchMaker::makeFnPrototypeBatchedForm() {
  for (auto & A : F->args()) {
    errs() << "Argument = " << A << "\n";
  }

}

} // tas namespace


