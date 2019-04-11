add_library(catch_LIB OBJECT test_main.cpp)

add_executable(tests
                simple_test.cpp)

target_link_libraries(tests
                      TASPass
                      catch_LIB
                      LLVMAsmParser
                      LLVMCore
                      LLVMAnalysis
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils)

add_test(NAME TASTests COMMAND tests)
