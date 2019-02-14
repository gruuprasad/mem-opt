#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <deque>
#include <map>
#include <string>

#include "Util.h"

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

void cloneLLVMObjects(Function &F) {
  ValueToValueMapTy VMap;
  Function *ClonedF = Function::Create(cast<FunctionType>(F.getValueType()), F.getLinkage(),
      F.getName() + "_cloned");
  ClonedF->copyAttributesFrom(&F);
  VMap[&F] = ClonedF;

  Function::arg_iterator ClonedFI = ClonedF->arg_begin();
  for (Function::const_arg_iterator J = F.arg_begin(); J != F.arg_end(); ++J) {
      ClonedFI->setName(J->getName());
      VMap[&*J] = &*ClonedFI++;
  }

  SmallVector<ReturnInst*, 16> Returns;
  CloneFunctionInto(ClonedF, &F, VMap, false, Returns, "Clone-");

  // Print results
  errs() << "old Function:\n" << F << "\n";
  errs() << "Total entries in VMap: " << VMap.size() << "\n";
  errs() << "new Function:\n" << *ClonedF << "\n";
  /*
  for (const auto &entry : VMap) {
    errs() << "Key:" << *entry.first << " Value:" << *entry.second << "\n";
  }*/
  ClonedF->eraseFromParent();
}

// Pass to collect loop info in a function
struct LoopDetails : public FunctionPass {
  static char ID;
  LoopDetails() : FunctionPass(ID) {}

  bool doInitialization(Module &M) override {
    tas::setAnnotationInFunctionObject(&M);
    return true;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LoopInfoWrapperPass>();
    AU.setPreservesAll();
  }

  bool runOnFunction(Function &F) override {
    if (!F.hasFnAttribute(tas::fn_mark)) 
      return false;

    cloneLLVMObjects(F);

    /*
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
    */
    return false;
  }
};

struct FunctionAttributeInfo : public FunctionPass {
  static char ID;
  FunctionAttributeInfo() : FunctionPass(ID) {}

  bool doInitialization(Module &M) override {
    tas::setAnnotationInFunctionObject(&M);
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
static RegisterPass<LoopDetails> X1("tas-loop-details", "Pass extracting loop info in a function",
                                   false,
                                   false);

} // Anonymous namespace
