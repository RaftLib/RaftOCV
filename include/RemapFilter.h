#pragma once

#include <raft>
#include <opencv2/core/mat.hpp>

class RemapFilter : public raft::kernel {
    cv::Mat map1, map2;
    cv::Mat newCameraMatrix;
public:
    RemapFilter();
    RemapFilter(const cv::Mat &map1, const cv::Mat &map2);
    RemapFilter(const cv::Mat& cameraMatrix,
                const cv::Mat& distCoeffs,
                const cv::Mat& R,
                cv::Size size, int m1type = CV_16SC2);

                raft::kstatus run() override;
};


