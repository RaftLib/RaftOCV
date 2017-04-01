#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <raft>
#include <DisplayFrameSink.h>
#include <ConvertColorFilter.h>
#include <StereoBMFilter.h>
#include <raftinc/print.tcc>
#include <DuplicateKernel.h>
#include <Metadata.h>
#include <JoinMetadataKernel.h>
#include <StereoCalibrateKernel.h>
#include <UndistortFilter.h>
#include "FindChessboardCornersKernel.h"

#include "ROIFilter.h"
#include "UVCSource.h"

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

