#pragma once
#include <raft>
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include "RaftOCV/utility/Metadata.h"

struct StereoBMFilter_p;

class StereoBMFilter : public raft::kernel
{
    friend struct StereoBMFilter_p;

    std::unique_ptr<StereoBMFilter_p> p;
    bool hasPCOutput = false;
    bool hasCalibInput = false;
public:
    typedef MetadataEnvelope<cv::UMat> output_t;
    StereoBMFilter();
    ~StereoBMFilter();

    raft::kstatus run() override;

    auto newCalibInput() -> decltype ( input["calib"]);
    auto newPointcloudOutput() -> decltype ( (*this)["pointcloud"]);

    void SetCalibration(const StereoCalibrationResults &results);
};


