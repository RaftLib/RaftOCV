project(RaftOCV C CXX)

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

include(package.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -std=c++14 -DL1D_CACHE_LINE_SIZE=64")

option(USE_ASAN "Use Address and UB sanitizers" OFF)
option(USE_PROFILER "Use Address and UB sanitizers" OFF)
option(BUILD_EXAMPLES "Build the example applications" ON)
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
        include/FromVector.h
        include/VectorizeData.h
        src/UVCSource.cpp include/UVCSource.h
        src/ROIFilter.cpp include/ROIFilter.h
        src/ConvertColorFilter.cpp include/ConvertColorFilter.h
        src/DisplayFrameSink.cpp include/DisplayFrameSink.h
        src/HeatmapFilter.cpp include/HeatmapFilter.h
        src/UndistortFilter.cpp include/UndistortFilter.h
        src/FindChessboardCornersKernel.cpp include/FindChessboardCornersKernel.h
        src/Metadata.cpp include/Metadata.h
        src/JoinMetadataKernel.cpp include/JoinMetadataKernel.h
        src/DuplicateKernel.cpp include/DuplicateKernel.h
        src/NullSink.cpp include/NullSink.h
        src/StereoCalibrateKernel.cpp include/StereoCalibrateKernel.h
        src/VideoCaptureSink.cpp include/VideoCaptureSink.h
        src/VideoCaptureSource.cpp include/VideoCaptureSource.h
        src/RemapFilter.cpp include/RemapFilter.h
        src/CalibrateKernel.cpp include/CalibrateKernel.h
        src/CombineFilter.cpp include/CombineFilter.h
        include/DropFilter.h
        src/StereoSubgraph.cpp include/StereoSubgraph.h
        src/SplitFilter.cpp include/SplitFilter.h
        include/NullSource.h
        src/CalibrationTypes.cpp include/CalibrationTypes.h
        include/KeyListener.h
        src/PoolAllocator.cpp include/PoolAllocator.h
        src/CannyEdgeKernel.cpp include/CannyEdgeKernel.h
        include/PoseEstimator.h src/PoseEstimator.cpp
        src/StabalizeVideo.cpp include/StabalizeVideo.h
        DiffMask.cpp DiffMask.h include/KernelTemplate.h
        WarpAffine.cpp WarpAffine.h
        )

IF(USE_PCL)
    LIST(APPEND RAFTOCV_SRC src/StereoBMFilter.cpp include/StereoBMFilter.h
            src/PointCloudFilter.cpp include/PointCloudFilter.h
            src/PointCloudViewer.cpp include/PointCloudViewer.h)
ENDIF()
add_library(RaftOCV STATIC ${RAFTOCV_SRC})

find_package(Threads)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
#find_package(OpenPose REQUIRED)

find_library(RAFT_LIB raftd)

message("!!!!!!! ${RAFT_LIB}")
target_link_libraries(RaftOCV
        ${RAFT_LIB}
        Qt5::Core Qt5::Gui Qt5::Widgets
        v4l2
        opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs opencv_calib3d opencv_video opencv_videoio opencv_cudabgsegm
        ${PCL_LIBRARIES}
        GL
        ${CMAKE_THREAD_LIBS_INIT})

IF(USE_PCL)
    find_package(PCL 1.8 REQUIRED)
    target_link_libraries(RaftOCV ${PCL_LIBRARIES} OpenNI2 glfw )
ENDIF()

target_link_libraries(RaftOCV)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()