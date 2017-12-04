#include <opencv2/core/mat.hpp>
#include <src/RaftOCV/utility/Metadata.h>
#include <opencv2/imgproc.hpp>
#include "CannyEdgeKernel.h"

raft::kstatus CannyEdgeKernel::run() {
    MetadataEnvelope<cv::Mat> out, img_in;
    input["0"].pop(img_in);
    out.metadata = img_in.Metadata();
    cv::Canny(img_in, out, threshold1, threshold2, apertureSize, L2gradient);

    output["0"].push(out);
    return raft::proceed;
}

CannyEdgeKernel::CannyEdgeKernel(double threshold1, double threshold2, int apertureSize, bool L2gradient) : threshold1(
        threshold1), threshold2(threshold2), apertureSize(apertureSize), L2gradient(L2gradient) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}
