add_library(TASPass
            CacheUsageAnalysisPass.cpp)

target_link_libraries(TASPass
                      LLVMCore
                      common)

