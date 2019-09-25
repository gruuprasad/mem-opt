add_executable(tasopt tasopt.cpp)

target_link_libraries(tasopt
                      tasopt_src
                      TASPass
                      LLVMCore
                      LLVMAnalysis
                      LLVMAsmParser
                      LLVMBitWriter
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils)



target_include_directories(tasopt PUBLIC src)
