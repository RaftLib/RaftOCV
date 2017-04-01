
#include <opencv2/imgproc.hpp>
#include "RemapFilter.h"
#include "Metadata.h"

RemapFilter::RemapFilter(const cv::Mat &map1, const cv::Mat &map2) : RemapFilter() {
    this->map1 = map1;
    this->map2 = map2;
}

raft::kstatus RemapFilter::run() {
    MetadataEnvelope<cv::Mat> in;
    input["0"].pop(in);
    MetadataEnvelope<cv::Mat> out(in.Metadata());

    cv::remap(in, out, map1, map2, cv::INTER_LINEAR);

    output["0"].push(out);

    return raft::proceed;
}

RemapFilter::RemapFilter(const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, const cv::Mat &R,
                         cv::Size size, int m1type) : RemapFilter() {
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix, size, m1type, map1, map2);
}

RemapFilter::RemapFilter() {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}
