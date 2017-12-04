cmake_minimum_required(VERSION 3.5)

project(RaftOCV C CXX)

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

include_directories( src src/RaftOCV/ui ${CGET_INSTALL_DIR}/include/pcl-1.8 /usr/include/eigen3 ${QtPropertyBrowser_INCLUDE_DIRS} )
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
        src/RaftOCV.h
        src/RaftOCV/utility/FromVector.h
        src/RaftOCV/utility/VectorizeData.h
        src/RaftOCV/videoio/UVCSource.cpp src/RaftOCV/videoio/UVCSource.h
        src/RaftOCV/imgproc/ROIFilter.cpp src/RaftOCV/imgproc/ROIFilter.h
        src/RaftOCV/imgproc/ConvertColorFilter.cpp src/RaftOCV/imgproc/ConvertColorFilter.h
        src/RaftOCV/ui/DisplayFrameSink.cpp src/RaftOCV/ui/DisplayFrameSink.h
        src/RaftOCV/imgproc/HeatmapFilter.cpp src/RaftOCV/imgproc/HeatmapFilter.h
        src/RaftOCV/imgproc/UndistortFilter.cpp src/RaftOCV/imgproc/UndistortFilter.h
        src/RaftOCV/calib3d/FindChessboardCornersKernel.cpp src/RaftOCV/calib3d/FindChessboardCornersKernel.h
        src/RaftOCV/utility/Metadata.cpp src/RaftOCV/utility/Metadata.h
        src/RaftOCV/utility/JoinMetadataKernel.cpp src/RaftOCV/utility/JoinMetadataKernel.h
        src/RaftOCV/utility/DuplicateKernel.cpp src/RaftOCV/utility/DuplicateKernel.h
        src/RaftOCV/utility/NullSink.cpp src/RaftOCV/utility/NullSink.h
        src/RaftOCV/stereo/StereoCalibrateKernel.cpp src/RaftOCV/stereo/StereoCalibrateKernel.h
        src/RaftOCV/videoio/VideoCaptureSink.cpp src/RaftOCV/videoio/VideoCaptureSink.h
        src/RaftOCV/videoio/VideoCaptureSource.cpp src/RaftOCV/videoio/VideoCaptureSource.h
        src/RaftOCV/imgproc/RemapFilter.cpp src/RaftOCV/imgproc/RemapFilter.h
        src/RaftOCV/calib3d/CalibrateKernel.cpp src/RaftOCV/calib3d/CalibrateKernel.h
        src/RaftOCV/utility/CombineFilter.cpp src/RaftOCV/utility/CombineFilter.h
        src/RaftOCV/utility/DropFilter.h
        src/RaftOCV/stereo/StereoSubgraph.cpp src/RaftOCV/stereo/StereoSubgraph.h
        src/RaftOCV/imgproc/SplitFilter.cpp src/RaftOCV/imgproc/SplitFilter.h
        src/RaftOCV/utility/NullSource.h
        src/RaftOCV/calib3d/CalibrationTypes.cpp src/RaftOCV/calib3d/CalibrationTypes.h
        src/RaftOCV/ui/KeyListener.h
        src/RaftOCV/utility/PoolAllocator.cpp src/RaftOCV/utility/PoolAllocator.h
        src/RaftOCV/imgproc/CannyEdgeKernel.cpp src/RaftOCV/imgproc/CannyEdgeKernel.h
        src/RaftOCV/tracking/PoseEstimator.h src/RaftOCV/tracking/PoseEstimator.cpp
        src/RaftOCV/tracking/StabalizeVideo.cpp src/RaftOCV/tracking/StabalizeVideo.h
        src/RaftOCV/imgproc/DiffMask.cpp src/RaftOCV/imgproc/DiffMask.h src/KernelTemplate.h
        src/RaftOCV/imgproc/WarpAffine.cpp src/RaftOCV/imgproc/WarpAffine.h
        src/RaftOCV/tracking/BgModel.cpp src/RaftOCV/tracking/BgModel.h
        src/RaftOCV/tracking/DenseOpticalFlow.cpp src/RaftOCV/tracking/DenseOpticalFlow.h
        src/RaftOCV/ui/UIMap.cpp src/RaftOCV/ui/UIMap.h src/RaftOCV/ui/UIMap.ui
        src/RaftOCV/ui/objectcontroller.cpp src/RaftOCV/ui/objectcontroller.h
        src/RaftOCV/utility/OptionalPort.cpp src/RaftOCV/utility/OptionalPort.h
        )

IF(USE_PCL)
    LIST(APPEND RAFTOCV_SRC src/RaftOCV/stereo/StereoBMFilter.cpp src/RaftOCV/stereo/StereoBMFilter.h
            src/RaftOCV/pcl/PointCloudFilter.cpp src/RaftOCV/pcl/PointCloudFilter.h
            src/RaftOCV/pcl/PointCloudViewer.cpp src/RaftOCV/pcl/PointCloudViewer.h)
ENDIF()

find_package(Threads)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(OpenPose REQUIRED)

find_library(RAFT_LIB raft raftd)
find_path(RAFT_LIB_INCLUDES raft HINTS ${CGET_RaftLib_INSTALL_ROOT})

add_library(RaftOCV STATIC ${RAFTOCV_SRC})
target_include_directories(RaftOCV PUBLIC ${RAFT_LIB_INCLUDES} ${OpenCV_INCLUDE_DIRS})

target_link_libraries(RaftOCV
        ${RAFT_LIB}
        Qt5::Core Qt5::Gui Qt5::Widgets
        v4l2
	${OpenCV_LIBS}	
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
        EXPORT RaftOCV
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY src/ DESTINATION include
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

install(EXPORT RaftOCV DESTINATION lib/RaftOCV)
