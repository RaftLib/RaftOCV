//
// Created by justin on 3/12/17.
//

#include <opencv2/opencv.hpp>
#include "DisplayFrameSink.h"

raft::kstatus DisplayFrameSink::run() {
    auto &img_in = input["0"].template peek<cv::Mat>();

    cv::imshow(frameName, img_in);
    //cv::displayOverlay(frameName, std::to_string(Input_<InputT>::hertz.Hertz()));

    input["0"].unpeek();
    input["0"].recycle(1);
    cv::waitKey(1);

    return raft::proceed;
}

DisplayFrameSink::DisplayFrameSink(const std::string &frameName) : frameName(frameName) {
    //cv::namedWindow(frameName, cv::WINDOW_NORMAL);
    input.addPort<cv::Mat>("0");
}
