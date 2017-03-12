project(RaftOCV C CXX)

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

include(package.cmake)

include_directories( . )

find_package( Threads )
find_package(Qt5Core)
find_package(Qt5Gui)

add_library(RaftOCV STATIC
        UVCSource.cpp UVCSource.h
        ROIFilter.cpp ROIFilter.h
        ConvertColorFilter.cpp ConvertColorFilter.h
        DisplayFrameSink.cpp DisplayFrameSink.h
        StereoBMFilter.cpp StereoBMFilter.h
        HeatmapFilter.cpp HeatmapFilter.h
        )

target_link_libraries(RaftOCV raft opencv_core opencv_highgui opencv_imgproc
        opencv_imgcodecs Qt5::Gui v4l2 opencv_calib3d
        GL OpenNI2 glfw ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(examples)