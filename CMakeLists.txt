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

macro(ADD_SOURCES)
    foreach(src ${ARGV})
       list(APPEND RAFTOCV_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${src})
        message("Adding ${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    endforeach ()
    SET(RAFTOCV_SRC ${RAFTOCV_SRC} PARENT_SCOPE)
endmacro()

SET(RAFTOCV_SRC src/RaftOCV.h)

add_subdirectory(src/RaftOCV)

IF(USE_PCL)
    LIST(APPEND RAFTOCV_SRC src/RaftOCV/stereo/StereoBMFilter.cpp src/RaftOCV/stereo/StereoBMFilter.h
            src/RaftOCV/pcl/PointCloudFilter.cpp src/RaftOCV/pcl/PointCloudFilter.h
            src/RaftOCV/pcl/PointCloudViewer.cpp src/RaftOCV/pcl/PointCloudViewer.h)
ENDIF()

find_package(Threads)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

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
