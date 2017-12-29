#pragma once

#include <raft>
#include <opencv2/core/mat.hpp>

class RemapFilter : public raft::kernel {
    cv::UMat map1, map2;
    cv::UMat newCameraMatrix;
public:
    RemapFilter();
    RemapFilter(const cv::UMat &map1, const cv::UMat &map2);
    RemapFilter(const cv::UMat& cameraMatrix,
                const cv::UMat& distCoeffs,
                const cv::UMat& R,
                cv::Size size, int m1type = CV_16SC2);

                raft::kstatus run() override;
};


