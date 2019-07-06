add_library(TASPass SHARED BatchProcess.cpp
                           BatchProcessPass.cpp
                           BatchMaker.cpp
                           BatchMakerPass.cpp
                           CacheUsageAnalysis.cpp
                           CacheUsageAnalysisPass.cpp)

add_library(TASUtil STATIC ForLoop.cpp Util.cpp)

target_link_libraries(TASPass TASUtil)

install(TARGETS TASPass DESTINATION lib)
