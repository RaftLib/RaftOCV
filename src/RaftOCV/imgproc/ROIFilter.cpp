#include "ROIFilter.h"

#include <opencv2/opencv.hpp>
#include "src/RaftOCV/utility/Metadata.h"

ROIFilter::ROIFilter(const cv::Rect &ROI) : ROI(ROI) {
    input.addPort<MetadataEnvelope<cv::UMat>>("0");
    output.addPort<MetadataEnvelope<cv::UMat>>("0");
}

raft::kstatus ROIFilter::run() {
    MetadataEnvelope<cv::UMat> img_in;
    input["0"].pop(img_in);

    MetadataEnvelope<cv::UMat> out(img_in.Metadata());

    out = cv::UMat(img_in, this->ROI).clone();
    output["0"].push(out);
    return (raft::proceed);
}
