include(${CGET_CORE_DIR}.cget/core.cmake)

set(CGET_ALLOW_SYSTEM ON)

CGET_HAS_DEPENDENCY(OpenCV GITHUB opencv/opencv VERSION 3.1.0
        OPTIONS
        -DBUILD_WITH_STATIC_CRT:BOOL=OFF
        -DBUILD_opencv_java:BOOL=OFF
        -DBUILD_opencv_calib3d:BOOL=ON
        -DBUILD_opencv_videoio:BOOL=ON
        -DBUILD_opencv_video:BOOL=ON
        -DBUILD_opencv_highgui:BOOL=ON
        -DBUILD_opencv_python:BOOL=OFF
        -DBUILD_opencv_python2:BOOL=OFF
        -DBUILD_opencv_python3:BOOL=OFF
        -DBUILD_FAT_JAVA_LIB:BOOL=OFF
        -DBUILD_TESTS:BOOL=OFF
        -DBUILD_PERF_TESTS:BOOL=OFF
        -DBUILD_PACKAGE:BOOL=OFF
        -DBUILD_DOCS:BOOL=OFF
        -DBUILD_opencv_apps:BOOL=OFF
        -BUILD_SHARED_LIBS:BOOL=ON
        -DANDROID:BOOL=OFF
        -DWITH_VTK:BOOL=OFF
        -DWITH_WIN32UI:BOOL=OFF
        -DWITH_QT:BOOL=ON
        -DBUILD_ZLIB:BOOL=OFF )

CGET_HAS_DEPENDENCY(RaftLib GITHUB jdavidberger/RaftLib NO_FIND_PACKAGE)