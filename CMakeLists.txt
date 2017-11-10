project(RaftOCV C CXX)

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

include(package.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -std=c++14")

option(USE_ASAN "Use Address and UB sanitizers" OFF)
option(USE_PROFILER "Use Address and UB sanitizers" OFF)
option(BUILD_EXAMPLES "Build the example applications" OFF)
option(USE_PCL "Use PCL library" OFF)

include_directories( include ${CGET_INSTALL_DIR}/include/pcl-1.8 /usr/include/eigen3 )
IF(USE_PROFILER)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg")
ENDIF()

IF(USE_ASAN)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fstack-check -fsanitize=undefined")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=integer -Werror=inconsistent-missing-override")
    endif()
    MESSAGE("ASAN enabled")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")

SET(RAFTOCV_SRC
        UVCSource.cpp include/UVCSource.h
        ROIFilter.cpp include/ROIFilter.h
        ConvertColorFilter.cpp include/ConvertColorFilter.h
        DisplayFrameSink.cpp include/DisplayFrameSink.h
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
        RemapFilter.cpp include/RemapFilter.h
        CalibrateKernel.cpp include/CalibrateKernel.h
        CombineFilter.cpp include/CombineFilter.h
        include/DropFilter.h
        StereoSubgraph.cpp include/StereoSubgraph.h
        SplitFilter.cpp include/SplitFilter.h
        include/NullSource.h
        CalibrationTypes.cpp include/CalibrationTypes.h
        include/KeyListener.h
        PoolAllocator.cpp include/PoolAllocator.h)

IF(USE_PCL)
    LIST(APPEND RAFTOCV_SRC StereoBMFilter.cpp include/StereoBMFilter.h
            PointCloudFilter.cpp include/PointCloudFilter.h
            PointCloudViewer.cpp include/PointCloudViewer.h)
ENDIF()
add_library(RaftOCV STATIC ${RAFTOCV_SRC})

find_package(Threads)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

target_link_libraries(RaftOCV
        raft
        Qt5::Core Qt5::Gui Qt5::Widgets
        v4l2
        opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs opencv_calib3d opencv_video opencv_videoio
        ${PCL_LIBRARIES}
        GL OpenNI2 glfw ${CMAKE_THREAD_LIBS_INIT})

IF(USE_PCL)
    find_package(PCL 1.8 REQUIRED)
    target_link_libraries(RaftOCV ${PCL_LIBRARIES})
ENDIF()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()