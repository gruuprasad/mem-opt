add_library(TASPass SHARED CacheUsageAnalysisPass.cpp)

add_library(TASUtil SHARED
  CacheUsageAnalysis.cpp
  Util.cpp)

target_link_libraries(TASPass TASUtil)

install(TARGETS TASPass DESTINATION lib)
