add_library(catch_LIB OBJECT test_main.cpp)

add_executable(tests
                simple_test.cpp
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
                      TASUtil
                      catch_LIB
                      LLVMCore
                      LLVMAnalysis
                      LLVMAsmParser
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils)

add_test(NAME TASTests COMMAND tests)
