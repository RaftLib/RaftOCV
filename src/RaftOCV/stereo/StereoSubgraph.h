#pragma once

#include <opencv2/core/mat.hpp>
#include "RaftOCV/imgproc/SplitFilter.h"
#include "RaftOCV/utility/CombineFilter.h"
#include "RaftOCV/utility/JoinMetadataKernel.h"
#include "StereoBMFilter.h"
#include "RaftOCV/utility/DuplicateKernel.h"
#include "StereoCalibrateKernel.h"
#include "RaftOCV/imgproc/UndistortFilter.h"

class StereoSubgraph {
    SplitFilter split;
    CombineFilter combine;
    StereoBMFilter stereo;

    DuplicateKernel < MetadataEnvelope<cv::Mat> > dup, stereoDup, rightOut, leftOut;

public:
    UndistortFilter undistortR, undistortL;
    StereoSubgraph();
    StereoSubgraph(const StereoCalibrationResults& r);
    StereoSubgraph(const std::string& r);

    auto in(const char * name = "0") -> decltype( split[name]);
    auto newImageOutput() -> decltype ( dup["0"]);
    auto newStereoOutput() -> decltype ( stereoDup["0"]);
    auto newLeftOutput() -> decltype ( leftOut["0"]);
    auto newRightOutput() -> decltype ( rightOut["0"]);

    auto pointCloudOutput() -> decltype ( stereo["pointcloud"]);

    void connect(raft::map& m);
};


