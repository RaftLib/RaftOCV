#include "ROIFilter.h"

#include <opencv2/opencv.hpp>
#include "Metadata.h"

ROIFilter::ROIFilter(const cv::Rect &ROI) : ROI(ROI) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}

raft::kstatus ROIFilter::run() {
    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    MetadataEnvelope<cv::Mat> out(img_in.Metadata());

    out = cv::Mat(img_in, this->ROI).clone();
    output["0"].push(out);
    return (raft::proceed);
}
