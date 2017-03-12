#pragma once

#include <raft>
#include <opencv2/core/types.hpp>

class ROIFilter : public raft::kernel {
public:
    cv::Rect ROI;
    ROIFilter(const cv::Rect& ROI = {});

    raft::kstatus run() override;

};



