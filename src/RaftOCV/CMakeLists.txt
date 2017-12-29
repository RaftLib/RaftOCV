
add_subdirectory(imgproc)
add_subdirectory(utility)
add_subdirectory(tracking)

ADD_SOURCES(
        videoio/UVCSource
        ui/DisplayFrameSink
        calib3d/FindChessboardCornersKernel
        stereo/StereoCalibrateKernel
        videoio/VideoCaptureSink
        videoio/VideoCaptureSource
        calib3d/CalibrateKernel
        stereo/StereoSubgraph
        calib3d/CalibrationTypes
        ui/KeyListener
        ui/UIMap
        ui/objectcontroller
)
