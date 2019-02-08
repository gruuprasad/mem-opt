#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>

#include "Util.h"

using namespace llvm;

namespace tas {

void setAnnotationInFunctionObject(Module * M) {
  if (auto annotationList = M->getNamedGlobal("llvm.global.annotations")) {
    auto ca = cast<ConstantArray>(annotationList->getOperand(0));
    for (unsigned int i = 0; i < ca->getNumOperands(); ++i) {
      auto ca_struct =cast<ConstantStruct>(ca->getOperand(i));
      if (auto ca_func = dyn_cast<Function>(ca_struct->getOperand(0)->getOperand(0))) {
        auto ca_annotation = cast<ConstantDataArray>(
            cast<GlobalVariable>(ca_struct->getOperand(1)->getOperand(0))->getOperand(0));

        ca_func->addFnAttr(ca_annotation->getAsCString());
      }
    }
  }
}

}
