
add_subdirectory(imgproc)
add_subdirectory(utility)

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
        tracking/PoseEstimator
        tracking/StabalizeVideo
        tracking/BgModel
        tracking/DenseOpticalFlow
        ui/UIMap
        ui/objectcontroller
)
