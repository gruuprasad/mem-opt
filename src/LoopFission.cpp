#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include "Util.h"

using namespace llvm;

namespace {

const std::string fn_mark = "tas_batch";

/*
 * Sample TAS code
lookup.c

static inline int fast_flow_lookup(ip_addr_t l_ip, ip_addr_t r_ip, beui16_t l_p,
    beui16_t r_p, struct flextcp_pl_flowst **pfs)
{
  uint32_t h, i, j, eh, fid, ffid;
  struct flow_key key;
  struct flextcp_pl_flowhte *e;
  struct flextcp_pl_flowst *fs;
  key.local_ip = l_ip;
  key.remote_ip = r_ip;
  key.local_port = l_p;
  key.remote_port = r_p;
  h = flow_hash(&key);
  for (j = 0; j < FLEXNIC_PL_FLOWHT_NBSZ; j++) {
    i = (h + j) % FLEXNIC_PL_FLOWHT_ENTRIES;
    e = &pl_memory->flowht[i];
    ffid = e->flow_id;
    MEM_BARRIER();
    eh = e->flow_hash;
    fid = ffid & ((1 << FLEXNIC_PL_FLOWHTE_POSSHIFT) - 1);
    if ((ffid & FLEXNIC_PL_FLOWHTE_VALID) == 0 || eh != h) {
      continue;
    }
    MEM_BARRIER();
    fs = &pl_memory->flowst[fid];
    if (fs->local_ip.x == key.local_ip.x &&
        fs->remote_ip.x == key.remote_ip.x &&
        fs->local_port.x == key.local_port.x &&
        fs->remote_port.x == key.remote_port.x)
    {
      *pfs = &pl_memory->flowst[fid];
      return 0;
    }
  }
  return -1;
}
*/

struct LoopFission : public FunctionPass {
  static char ID;
  LoopFission() : FunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LoopInfoWrapperPass>();
    //AU.setPreservesAll();
  }
  
  bool doInitialization(Module &M) override {
    tas::setAnnotationInFunctionObject(&M);
    return true;
  }

  // Perform loop-fission on function annotated with tas attribute.
  bool runOnFunction(Function &F) override {
    if (!F.hasFnAttribute(fn_mark)) 
      return false;

    errs() << "LoopFission pass: " << F.getName() << "\n";
    tas::detectVarAnnotation(&F);
    /*
    LoopInfo *LI = &getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
    auto OrigLoop = *LI->begin();
    auto PreHeader = OrigLoop->getLoopPreheader();
    auto Header = OrigLoop->getHeader();
    if (OrigLoop->isLoopExiting(Header)) {
      errs() << "Header has exit branch\n";
    }
    */

    return false;
  }
};

char LoopFission::ID = 0;
static RegisterPass<LoopFission> X("loop-fission", "Pass replicating single loop into multiple instances",
                                   false,
                                   false);
} // Anonymous namespace
