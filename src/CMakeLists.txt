add_library(TASPass SHARED BatchProcess.cpp
                           BatchProcessPass.cpp
                           BatchMaker.cpp
                           BatchMakerPass.cpp
                           ForLoop.cpp
                           Util.cpp)

install(TARGETS TASPass DESTINATION lib)
