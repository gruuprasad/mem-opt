#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <deque>
#include <map>
#include <string>

using namespace llvm;

namespace tas {

static const std::string fn_mark = "tas_batch";
static const std::string var_mark = "tas_attr2";

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

// Pass to collect loop info in a function
struct LoopDetails : public FunctionPass {
  static char ID;
  LoopDetails() : FunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
    AU.addRequired<LoopInfoWrapperPass>();
  }

  bool runOnFunction(Function &F) override {
    if (!F.hasFnAttribute(tas::fn_mark)) 
      return false;

    errs() << "\nLoopFission pass: " << F.getName() << "\n";
    LoopInfo *LI = &getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
    LI->print(errs());
    auto loop = *LI->begin();
    errs() << "GetNumBlocks " << loop->getNumBlocks() << "\n";
    errs() << "GetLoopDepth " << loop->getLoopDepth() << "\n";

    if (auto ph = loop->getLoopPreheader()) {
      errs() << "Preheader:\n" << *ph << "\n";
    }

    errs() << "Loop Header:\n" << *loop->getHeader() << "\n";
    errs() << "Loop Header has exit instruction:" << loop->isLoopExiting(loop->getHeader()) << "\n";

    for (auto & bb : loop->blocks()) {
      errs() << *bb;
    }
    errs() << "\n";
    
    errs() << "Is Safe to clone " << (loop->isSafeToClone() ? "Yes": "No") << "\n";
    errs() << "Is loop simplify form " << (loop->isLoopSimplifyForm() ? "Yes": "No") << "\n";

    errs() << "Loop blocks in preorder\n";
    auto loopBlocks = LI->getLoopsInPreorder();
    for (auto & bb : loopBlocks) {
      errs() << "block " << *bb;
    }
    //assert(loop->getSubLoops().empty() &&
    //     "Loop to be cloned cannot have inner loop");
    return false;
  }
};

struct FunctionAttributeInfo : public FunctionPass {
  static char ID;
  FunctionAttributeInfo() : FunctionPass(ID) {}

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

  bool doInitialization(Module &M) override {
    setAnnotationInFunctionObject(&M);
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

char FunctionAttributeInfo::ID = 0;
static RegisterPass<FunctionAttributeInfo> X("tas-fn-attribute", "Pass detecting functions with TAS-related attribute",
                                      false /* Only looks at CFG */,
                                      false /* Analysis Pass */);

char LoopDetails::ID = 0;
static RegisterPass<LoopDetails> X1("eli", "Pass extracting loop info in a function",
                                   false,
                                   false);

} // Anonymous namespace
