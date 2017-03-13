//
// Created by justin on 3/12/17.
//

#include <opencv2/imgproc.hpp>
#include "UndistortFilter.h"

raft::kstatus UndistortFilter::run() {
    auto &img_in = input["0"].template peek<cv::Mat>();
    auto &out = output["0"].template allocate<cv::Mat>();

    cv::remap(img_in, out, map1, map2, cv::INTER_LINEAR);

    input["0"].unpeek();
    input["0"].recycle(1);
    output["0"].send();
    return (raft::proceed);
}

UndistortFilter::UndistortFilter(const cv::Mat &map1, const cv::Mat &map2) : map1(map1), map2(map2) {
    input.addPort<cv::Mat>("0");
    output.addPort<cv::Mat>("0");
}
