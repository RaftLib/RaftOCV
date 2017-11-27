project(RaftOCV C CXX)

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

include(package.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -std=c++14 -DL1D_CACHE_LINE_SIZE=64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(USE_ASAN "Use Address and UB sanitizers" OFF)
option(USE_PROFILER "Use Address and UB sanitizers" OFF)
option(BUILD_EXAMPLES "Build the example applications" ON)
option(USE_PCL "Use PCL library" OFF)

find_package(QtPropertyBrowser)

include_directories( src src/ui ${CGET_INSTALL_DIR}/include/pcl-1.8 /usr/include/eigen3 ${QtPropertyBrowser_INCLUDE_DIRS} )
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
        src/utility/FromVector.h
        src/utility/VectorizeData.h
        src/videoio/UVCSource.cpp src/videoio/UVCSource.h
        src/imgproc/ROIFilter.cpp src/imgproc/ROIFilter.h
        src/imgproc/ConvertColorFilter.cpp src/imgproc/ConvertColorFilter.h
        src/ui/DisplayFrameSink.cpp src/ui/DisplayFrameSink.h
        src/imgproc/HeatmapFilter.cpp src/imgproc/HeatmapFilter.h
        src/imgproc/UndistortFilter.cpp src/imgproc/UndistortFilter.h
        src/calib3d/FindChessboardCornersKernel.cpp src/calib3d/FindChessboardCornersKernel.h
        src/utility/Metadata.cpp src/utility/Metadata.h
        src/utility/JoinMetadataKernel.cpp src/utility/JoinMetadataKernel.h
        src/utility/DuplicateKernel.cpp src/utility/DuplicateKernel.h
        src/utility/NullSink.cpp src/utility/NullSink.h
        src/stereo/StereoCalibrateKernel.cpp src/stereo/StereoCalibrateKernel.h
        src/videoio/VideoCaptureSink.cpp src/videoio/VideoCaptureSink.h
        src/videoio/VideoCaptureSource.cpp src/videoio/VideoCaptureSource.h
        src/imgproc/RemapFilter.cpp src/imgproc/RemapFilter.h
        src/calib3d/CalibrateKernel.cpp src/calib3d/CalibrateKernel.h
        src/utility/CombineFilter.cpp src/utility/CombineFilter.h
        src/utility/DropFilter.h
        src/stereo/StereoSubgraph.cpp src/stereo/StereoSubgraph.h
        src/imgproc/SplitFilter.cpp src/imgproc/SplitFilter.h
        src/utility/NullSource.h
        src/calib3d/CalibrationTypes.cpp src/calib3d/CalibrationTypes.h
        src/ui/KeyListener.h
        src/utility/PoolAllocator.cpp src/utility/PoolAllocator.h
        src/imgproc/CannyEdgeKernel.cpp src/imgproc/CannyEdgeKernel.h
        src/tracking/PoseEstimator.h src/tracking/PoseEstimator.cpp
        src/tracking/StabalizeVideo.cpp src/tracking/StabalizeVideo.h
        src/imgproc/DiffMask.cpp src/imgproc/DiffMask.h src/KernelTemplate.h
        src/imgproc/WarpAffine.cpp src/imgproc/WarpAffine.h
        src/tracking/BgModel.cpp src/tracking/BgModel.h
        src/tracking/DenseOpticalFlow.cpp src/tracking/DenseOpticalFlow.h
        src/ui/UIMap.cpp src/ui/UIMap.h src/ui/UIMap.ui
        src/ui/objectcontroller.cpp src/ui/objectcontroller.h
        src/utility/OptionalPort.cpp src/utility/OptionalPort.h
        )

IF(USE_PCL)
    LIST(APPEND RAFTOCV_SRC src/stereo/StereoBMFilter.cpp src/stereo/StereoBMFilter.h
            src/pcl/PointCloudFilter.cpp src/pcl/PointCloudFilter.h
            src/pcl/PointCloudViewer.cpp src/pcl/PointCloudViewer.h)
ENDIF()

find_package(Threads)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(OpenPose REQUIRED)

find_library(RAFT_LIB raft raftd)

add_library(RaftOCV STATIC ${RAFTOCV_SRC})

target_link_libraries(RaftOCV
        ${RAFT_LIB}
        Qt5::Core Qt5::Gui Qt5::Widgets
        v4l2
        opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs opencv_calib3d opencv_video opencv_videoio
        opencv_cudabgsegm opencv_cudaoptflow
        ${PCL_LIBRARIES}
        GL
        ${CMAKE_THREAD_LIBS_INIT})

IF(USE_PCL)
    find_package(PCL 1.8 REQUIRED)
    target_link_libraries(RaftOCV ${PCL_LIBRARIES} OpenNI2 glfw )
ENDIF()

target_link_libraries(RaftOCV QtPropertyBrowser openpose)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(TARGETS RaftOCV
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY src/ DESTINATION include/RaftOCV
        PATTERN "**/*.h"
        PATTERN "*.c*" EXCLUDE
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
        GROUP_EXECUTE GROUP_READ)

# Compute installation prefix relative to this file
configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/RaftOCVConfig.cmake.in
        ${CMAKE_BINARY_DIR}/cmake/RaftOCVConfig.cmake @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/cmake/RaftOCVConfig.cmake
        DESTINATION lib/cmake)
