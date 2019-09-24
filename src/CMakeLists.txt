add_subdirectory(PassWrapper)

add_library(tasopt_src
            BatchMaker.cpp
            BatchProcess.cpp
            BlockPredication.cpp
            CacheUsageInfo.cpp
            ForLoop.cpp
            PacketPathAnalysis.cpp
            ToolUtil.cpp
            Util.cpp)
