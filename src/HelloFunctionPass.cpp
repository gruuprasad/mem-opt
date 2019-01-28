#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <string>

using namespace llvm;

namespace tas {

const std::string fn_mark = "tas_attr1";
const std::string var_mark = "tas_attr2";

std::string IRTypeToString(Instruction & I) {
  if (isa<PHINode>(I))
    return std::string("  PHI instruction");
  else if (isa<AllocaInst>(I))
    return std::string(" Alloca instruction");
  else if (isa<LoadInst>(I))
    return std::string(" Load instruction");
  else if (isa<StoreInst>(I))
    return std::string(" Store instruction");
  else if (isa<GetElementPtrInst>(I))
    return std::string(" GetElementPtr instruction");

  return std::string();
}

}

namespace {

struct HelloFunction : public FunctionPass {
  static char ID;
  HelloFunction() : FunctionPass(ID) {}

  // Annotation list structure in IR: stored as array of struct
  // ConstantArray : [Size x struct]
  // struct definition: [Function  ptr, GlobalVariable ptr, GlobalVariable ptr, i32]
  // @llvm.global.annotations = [N x {i8*, i8*, i8*, i32}]
  // N - number of global annotations in a module
  // Struct members details:
  // i8* - Function pointer
  // i8* - Pointer to annotation string
  // i8* - Pointer to file name string
  // i32 - line number of annotation in source file
  //
  // This function goes through annotation list stored as global variable.
  // For each annotation, it adds annotation as attribute in corresponding
  // Function object so that other passes can use the presence of this attribute
  // to do transformation.
  void setTASFnAnnotationinFunction(Module * M) {
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

  bool doInitialization(Module &M) override {
    setTASFnAnnotationinFunction(&M);
    return true;
  }

  bool runOnFunction(Function &F) override {
    errs() << F.getName() << ": ";
    errs() << "No of arguments: " << F.arg_size();
    errs() << ", Has TAS Attribute = " << (F.hasFnAttribute(tas::fn_mark) ? "Yes" : "No") << "\n";

    /*
    // Iterate over Basic Blocks in a function
    for (BasicBlock &BB : F) {
      errs() << "Basic Block (name=" << BB.getName() << " ) has " << BB.size() << " instructions.\n";
      // Iterate only over phi nodes
      unsigned int phi_count = 0;
      for (auto && pi : BB.phis()) {
        phi_count++;
      }
      if (phi_count)
        errs() << "Number of phi instructions = " << phi_count << "\n";
    }

    errs() << "Op1: iterate over instructions in a function\n";
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
      errs() << *I << tas::IRTypeToString(*I) << "\n";

    SmallPtrSet<Instruction*, 32> worklist;
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
      worklist.insert(&*I);
    */

    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};

char HelloFunction::ID = 0;
static RegisterPass<HelloFunction> X("hello", "Hello Function Pass",
                                      false /* Only looks at CFG */,
                                      false /* Analysis Pass */);
} // Anonymous namespace
