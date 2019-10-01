#include "PacketPathAnalysis.h"

#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Analysis/PostDominators.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Debug.h>

#include <algorithm>
#include <cassert>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "tas"

namespace tas {

void PacketPathAnalysis::calculate() {
  // Ordering is implicit when only 2 blocks present.
  if (F->getBasicBlockList().size() <= 2)
    return;

  ReversePostOrderTraversal<Function*> RPOT(F);
  for (auto B = RPOT.begin(); B != RPOT.end(); ++B) {
    auto PredSize = pred_size(*B);
    auto SuccSize = succ_size(*B);
    // paths converge at BB, create new mask id for this block.
    if (PredSize > 1) {
      BlockToMaskID[*B] = ++MaskIDCounter;
    }

    // Create new mask id for each successor block.
    // Paths diverge from BB
    if (SuccSize > 1) {
      for (auto * SuccBB : successors(*B)) {
        BlockToMaskID[SuccBB] = ++MaskIDCounter;
      }
      continue;
    }

    if (SuccSize == 1) {
      if (PredSize == 1) {
        BlockToMaskID[*succ_begin(*B)] = MaskIDCounter;
      } else if (PredSize > 1) {
        // Paths converge hence successor gets new mask id.  
        BlockToMaskID[*succ_begin(*B)] = ++MaskIDCounter;
      }
    }
  }
}

void PacketPathAnalysis::dumpDebugDataToConsole() {
  errs() << "Block to mask id map\n";
  for (auto & E : BlockToMaskID) {
    auto & BB = E.getFirst();
    BB->printAsOperand(errs());
    errs() << " : ";
    auto & ID = E.getSecond();
    errs() << to_string(ID) << "\n";
  }
}

}
