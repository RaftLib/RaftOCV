#include <opencv2/core/mat.hpp>
#include <src/RaftOCV/utility/Metadata.h>
#include <opencv2/imgproc.hpp>
#include "CannyEdgeKernel.h"

CannyEdgeFunctor::CannyEdgeFunctor(double threshold1, double threshold2, int apertureSize, bool L2gradient) : threshold1(
        threshold1), threshold2(threshold2), apertureSize(apertureSize), L2gradient(L2gradient) {
}

MetadataEnvelope<cv::UMat> CannyEdgeFunctor::operator()(const MetadataEnvelope<cv::UMat> &img_in) {
    MetadataEnvelope<cv::UMat> out;
    out.metadata = img_in.Metadata();
    cv::Canny(img_in, out, threshold1, threshold2, apertureSize, L2gradient);
    return out;
}
