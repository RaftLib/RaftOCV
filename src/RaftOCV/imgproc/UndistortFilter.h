#pragma once
#include <raft>
#include <opencv2/core/mat.hpp>
#include "RaftOCV/calib3d/CalibrateKernel.h"

class StereoCalibrationResults;

class UndistortFilter : public raft::kernel {
    cv::Mat map1, map2;

    bool isStereo = false;
    bool isRightCamera;
    bool expectCalibKernel = true;

    void calculate(const StereoCalibrationResults& calib);
    void calculate(const SingleCalibrationCameraResults& calib);
    void setupPorts();
public:
    UndistortFilter();
    UndistortFilter(bool isRightCamera);
    UndistortFilter(const cv::Mat &map1, const cv::Mat &map2);
    UndistortFilter(const SingleCalibrationCameraResults& calib);
    UndistortFilter(bool isRightCamera, const StereoCalibrationResults& calib);
    raft::kstatus run() override;
};
