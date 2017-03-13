project(RaftOCV C CXX)

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

include(package.cmake)

include_directories( include )

find_package( Threads )
find_package(Qt5Core)
find_package(Qt5Gui)

add_library(RaftOCV STATIC
        UVCSource.cpp include/UVCSource.h
        ROIFilter.cpp include/ROIFilter.h
        ConvertColorFilter.cpp include/ConvertColorFilter.h
        DisplayFrameSink.cpp include/DisplayFrameSink.h
        StereoBMFilter.cpp include/StereoBMFilter.h
        HeatmapFilter.cpp include/HeatmapFilter.h
        UndistortFilter.cpp include/UndistortFilter.h
        FindChessboardCornersKernel.cpp include/FindChessboardCornersKernel.h
        Metadata.cpp include/Metadata.h
        JoinMetadataKernel.cpp include/JoinMetadataKernel.h
        DuplicateKernel.cpp include/DuplicateKernel.h
        NullSink.cpp include/NullSink.h
        StereoCalibrateKernel.cpp include/StereoCalibrateKernel.h
        VideoCaptureSink.cpp include/VideoCaptureSink.h
        VideoCaptureSource.cpp include/VideoCaptureSource.h
        )

target_link_libraries(RaftOCV raftd opencv_core opencv_highgui opencv_imgproc
        opencv_imgcodecs Qt5::Gui v4l2 opencv_calib3d
        GL OpenNI2 glfw ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(examples)