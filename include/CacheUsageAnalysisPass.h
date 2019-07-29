/** @file CacheUsageAnalysisPass.h
 * \brief A pass to analyze the cache usage.
 */
#ifndef TAS_CACHEUSAGEANALYSISPASS_H
#define TAS_CACHEUSAGEANALYSISPASS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

namespace tas {

/// This pass computes the number of cache lines used by the method of interest.
///
/// Method of interest has to be annotated as \ref TAS_ANALYZE_CACHE in method declaration.
/// 
/// Example code annotation:
/// \code int process_packet(int a) TAS_ANALYZE_CACHE; \endcode
class CacheUsageAnalysisPass : public llvm::FunctionPass {
  unsigned CacheLineSize; ///< Cache line size in bytes (default - 64 bytes)
public:
  static char ID;
  CacheUsageAnalysisPass(unsigned N = 64) : FunctionPass(ID), CacheLineSize(N) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
 
  bool doInitialization(llvm::Module &M) override;

  bool runOnFunction(llvm::Function &F) override;
};

}

#endif
