add_library(catch_LIB OBJECT test_main.cpp)

add_executable(tests
                simple_test.cpp)

target_link_libraries(tests
                      catch_LIB
                      LLVMCore
                      LLVMSupport
                      LLVMIRReader)

add_test(TASTests tests)
