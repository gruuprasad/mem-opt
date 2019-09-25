add_library(catch_LIB OBJECT test_main.cpp)

add_executable(tests
                batchMaker_test.cpp
                blockPredication_test.cpp
                cacheAnalysis_test.cpp
                common_test.cpp
                utils_test.cpp)

set(LLVM_LINK_COMPONENTS
  Analysis
  AsmParser
  Core
  Support
  Passes
  IRReader
  TransformUtils)

target_link_libraries(tests
                      tasopt_src
                      catch_LIB
                      LLVMCore
                      LLVMAnalysis
                      LLVMAsmParser
                      LLVMBitWriter
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils
                      glog::glog)

add_test(NAME TASTests COMMAND tests)
