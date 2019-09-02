set(HEADERS BatchMakerPass.h BatchProcessPass.h CacheUsageAnalysisPass.h)
add_library(TASPass
            ${HEADERS}
            BatchMakerPass.cpp
            BatchProcessPass.cpp
            CacheUsageAnalysisPass.cpp)

target_link_libraries(TASPass
                      LLVMCore
                      common)

