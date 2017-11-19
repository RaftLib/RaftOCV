#pragma once


#include <opencv2/core/types.hpp>
#include <raft>

class WarpAffine : public raft::kernel {
    cv::Size destSize;

    int flags = cv::INTER_LINEAR;
    int borderMode = cv::BORDER_CONSTANT;
    cv::Scalar borderValue = cv::Scalar();

public:
    raft::kstatus run() override;

    WarpAffine(const cv::Size &destSize = {},
               int flags = cv::INTER_LINEAR,
               int borderMode = cv::BORDER_CONSTANT,
               const cv::Scalar& borderValue = cv::Scalar());
};


