add_library(tasopt_src
            BatchMaker.cpp
            BlockPredication.cpp
            CacheUsageInfo.cpp
            ForLoop.cpp
            ForLoopV2.cpp
            LoopSplitter.cpp
            PacketPathAnalysis.cpp
            ToolUtil.cpp
            Util.cpp)

add_subdirectory(PassWrapper)
