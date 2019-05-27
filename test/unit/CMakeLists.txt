add_library(catch_LIB OBJECT test_main.cpp)

add_executable(tests
                simple_test.cpp)

set(LLVM_LINK_COMPONENTS
  Analysis
  AsmParser
  Core
  Support
  Passes
  IRReader
  TransformUtils)

target_link_libraries(tests
                      TASPass
                      catch_LIB
                      LLVMCore
                      LLVMAnalysis
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils)

add_test(NAME TASTests COMMAND tests)
