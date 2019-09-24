/** @file CacheUsageAnalysisPass.h
 * \brief A pass to analyze the cache usage.
 */
#ifndef TAS_CACHEUSAGEANALYSISPASS_H
#define TAS_CACHEUSAGEANALYSISPASS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>

#include "CacheUsageInfo.h"

namespace tas {

/// This pass computes the number of cache lines used by the method of interest.
///
/// Method of interest has to be annotated as \ref TAS_ANALYZE_CACHE in method declaration.
/// 
/// Example code annotation:
///
///  ```
///   int process_packet(int a) TAS_ANALYZE_CACHE;
///  ```

class CacheUsageAnalysis : public llvm::AnalysisInfoMixin<CacheUsageAnalysis> {
  friend AnalysisInfoMixin<CacheUsageAnalysis>;
  static llvm::AnalysisKey Key;
  unsigned CacheLineSize;
public:
  using Result = CacheUsageInfo;

  CacheUsageAnalysis(int N) : CacheLineSize(N) {}

  CacheUsageInfo run(llvm::Function & F, llvm::FunctionAnalysisManager &AM);
};

class CacheUsageAnalysisPass : public llvm::FunctionPass {
  unsigned CacheLineSize; ///< Cache line size in bytes (default - 64 bytes)
  CacheUsageInfo CI;
public:
  static char ID;
  CacheUsageAnalysisPass(unsigned N = 64) : FunctionPass(ID), CacheLineSize(N), CI(N) {}

  const CacheUsageInfo& getCacheUsageInfo() { return CI; }

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
 
  bool doInitialization(llvm::Module &M) override;

  bool runOnFunction(llvm::Function &F) override;
};

/// FIXME This method runs the analysis pass in a pipeline.
/// How to get the result of the analysis?
/// In other pass, one can ue `getAnalysis<CacheUsageAnalysisPass>()` to
/// use the analysis result.
void runCacheAnalysisPass(llvm::Module * M, unsigned CacheLineSize);

}

#endif
