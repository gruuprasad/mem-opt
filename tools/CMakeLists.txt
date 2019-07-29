add_executable(tasopt tasopt.cpp)

target_link_libraries(tasopt
                      TASPass
                      cacheanalysis
                      LLVMCore
                      LLVMAnalysis
                      LLVMAsmParser
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils)
