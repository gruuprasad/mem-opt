add_library(TASPass SHARED CacheUsageAnalysis.cpp)

add_library(TASUtil STATIC ForLoop.cpp Util.cpp)

target_link_libraries(TASPass TASUtil)

install(TARGETS TASPass DESTINATION lib)
