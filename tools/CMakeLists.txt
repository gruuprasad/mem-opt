add_executable(tasopt tasopt.cpp)

target_link_libraries(tasopt
                      TASPass
                      batchMaker
                      cacheUsageInfo
                      LLVMCore
                      LLVMAnalysis
                      LLVMAsmParser
                      LLVMSupport
                      LLVMIRReader
                      LLVMTransformUtils)



target_include_directories(tasopt PUBLIC src)
