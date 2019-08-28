add_library(catch_LIB OBJECT test_main.cpp)

add_executable(tests
                cacheAnalysis_test.cpp)

set(LLVM_LINK_COMPONENTS
  Analysis
  AsmParser
  Core
  Support
  Passes
  IRReader
  TransformUtils)

target_link_libraries(tests
                      cacheUsageInfo
                      catch_LIB
                      common
                      LLVMCore
                      LLVMAnalysis
                      LLVMAsmParser
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils
                      glog::glog)

add_test(NAME TASTests COMMAND tests)
