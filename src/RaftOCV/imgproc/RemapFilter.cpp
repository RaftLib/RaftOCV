
#include <opencv2/imgproc.hpp>
#include "RemapFilter.h"
#include "src/RaftOCV/utility/Metadata.h"

RemapFilter::RemapFilter(const cv::UMat &map1, const cv::UMat &map2) : RemapFilter() {
    this->map1 = map1;
    this->map2 = map2;
}

raft::kstatus RemapFilter::run() {
    MetadataEnvelope<cv::UMat> in;
    input["0"].pop(in);
    MetadataEnvelope<cv::UMat> out(in.Metadata());

    cv::remap(in, out, map1, map2, cv::INTER_LINEAR);

    output["0"].push(out);

    return raft::proceed;
}

RemapFilter::RemapFilter(const cv::UMat &cameraMatrix, const cv::UMat &distCoeffs, const cv::UMat &R,
                         cv::Size size, int m1type) : RemapFilter() {
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix, size, m1type, map1, map2);
}

RemapFilter::RemapFilter() {
    input.addPort<MetadataEnvelope<cv::UMat>>("0");
    output.addPort<MetadataEnvelope<cv::UMat>>("0");
}
