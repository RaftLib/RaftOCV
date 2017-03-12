//
// Created by justin on 3/12/17.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "ConvertColorFilter.h"

raft::kstatus ConvertColorFilter::run() {
    auto &img_in = input["0"].template peek<cv::Mat>();
    auto &out = output["0"].template allocate<cv::Mat>();

    cv::cvtColor(img_in, out, ConversionType);

    input["0"].unpeek();
    input["0"].recycle(1);
    output["0"].send();

    return raft::proceed;
}

ConvertColorFilter::ConvertColorFilter(int16_t ConversionType) : ConversionType(ConversionType) {
    input.addPort<cv::Mat>("0");
    output.addPort<cv::Mat>("0");
}
