#include <opencv2/imgproc.hpp>
#include "WarpAffine.h"
#include "Metadata.h"

WarpAffine::WarpAffine(const cv::Size &destSize,
                       int flags,
                       int borderMode,
                       const cv::Scalar& borderValue) : destSize(destSize), flags(flags), borderMode(borderMode), borderValue(borderValue) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    input.addPort<cv::Mat>("tx");

    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}

raft::kstatus WarpAffine::run() {
    MetadataEnvelope<cv::Mat> img_in;
    if(input["0"].size() == 0 || input["tx"].size() == 0)
        return raft::proceed;

    input["0"].pop(img_in);

    cv::Mat tx;
    input["tx"].pop(tx);

    MetadataEnvelope<cv::Mat> out(img_in.Metadata());

    cv::warpAffine(img_in, out, cv::Mat(tx.inv(cv::DECOMP_SVD)).rowRange(0, 2), destSize, flags, borderMode, borderValue);

    output["0"].push(out);

    return raft::proceed;
}
