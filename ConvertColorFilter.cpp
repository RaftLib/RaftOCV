//
// Created by justin on 3/12/17.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "ConvertColorFilter.h"
#include "Metadata.h"

raft::kstatus ConvertColorFilter::run() {
    MetadataEnvelope<cv::Mat> out, img_in;
    input["0"].pop(img_in);
    out.metadata = img_in.Metadata();
    cv::cvtColor(img_in, out, ConversionType);

    output["0"].push(out);
    return raft::proceed;
}

ConvertColorFilter::ConvertColorFilter(int16_t ConversionType) : ConversionType(ConversionType) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}
