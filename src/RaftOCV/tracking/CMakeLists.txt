
ADD_SOURCES(
        StabalizeVideo
        BgModel
        DenseOpticalFlow
)

IF(USE_OPENPOSE)
    ADD_SOURCES(PoseEstimator)
ENDIF()