add_library(TASPass
            BatchMakerPass.cpp
            BatchProcessPass.cpp
            CacheUsageAnalysisPass.cpp)

target_link_libraries(TASPass
                      LLVMCore
                      common)

