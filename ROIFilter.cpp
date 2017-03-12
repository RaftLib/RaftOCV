#include "ROIFilter.h"

#include <opencv2/opencv.hpp>

ROIFilter::ROIFilter(const cv::Rect &ROI) : ROI(ROI) {
    input.addPort<cv::Mat>("0");
    output.addPort<cv::Mat>("0");
}

raft::kstatus ROIFilter::run() {
    auto &img_in = input["0"].template peek<cv::Mat>();
    auto &out = output["0"].template allocate<cv::Mat>();

    out = cv::Mat(img_in, this->ROI).clone();

    input["0"].unpeek();
    input["0"].recycle(1);
    output["0"].send();
    return (raft::proceed);
}
