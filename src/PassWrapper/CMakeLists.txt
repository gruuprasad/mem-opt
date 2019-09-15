set(HEADERS BatchMakerPass.h CacheUsageAnalysisPass.h)
add_library(TASPass SHARED
            ${HEADERS}
            BatchMakerPass.cpp
            CacheUsageAnalysisPass.cpp)

target_link_libraries(TASPass
                      batchMaker
                      LLVMCore
                      common)

