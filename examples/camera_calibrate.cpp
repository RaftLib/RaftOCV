#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <raft>
#include <ui/DisplayFrameSink.h>
#include <imgproc/ConvertColorFilter.h>
#include <stereo/StereoBMFilter.h>
#include <raftinc/print.tcc>
#include <utility/DuplicateKernel.h>
#include <utility/Metadata.h>
#include <utility/JoinMetadataKernel.h>
#include <stereo/StereoCalibrateKernel.h>
#include <imgproc/UndistortFilter.h>
#include "calib3d/FindChessboardCornersKernel.h"

#include "imgproc/ROIFilter.h"
#include "videoio/UVCSource.h"

int main() {
    using p_out = raft::print< SingleCalibrationCameraResults, '\n' >;

    uint32_t imgWidth = 640, imgHeight = 400;

    UVCSource src;
    ConvertColorFilter convert(CV_YUV2BGR_YUY2);
    FindChessboardCornersKernel findChessboard(cv::Size(6, 8));
    DisplayFrameSink display("Video"), displayCalibrated("Calibrated");
    CalibrateKernel cal(cv::Size(imgWidth, imgHeight), cv::Size(6, 8));
    UndistortFilter undistort;
    DuplicateKernel< MetadataEnvelope<cv::Mat> > dup(2);
    p_out printer;

    raft::map m;

    m += src >> convert >> dup;
    m += dup["0"] >> findChessboard["0"]["debug"] >> display;
    m += findChessboard["0"]["0"] >> cal >> undistort["calib"];

    m += dup["1"] >> undistort["0"] >> displayCalibrated;

    m.exe();
    return( EXIT_SUCCESS );
}

