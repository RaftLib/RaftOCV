#include <opencv2/imgproc.hpp>
#include "WarpAffine.h"
#include "src/RaftOCV/utility/Metadata.h"

WarpAffine::WarpAffine(const cv::Size &destSize,
                       int flags,
                       int borderMode,
                       const cv::Scalar& borderValue) : destSize(destSize), flags(flags), borderMode(borderMode), borderValue(borderValue) {
    input.addPort<MetadataEnvelope<cv::UMat>>("0");
    input.addPort<cv::UMat>("tx");

    output.addPort<MetadataEnvelope<cv::UMat>>("0");
}

raft::kstatus WarpAffine::run() {
    MetadataEnvelope<cv::UMat> img_in;
    std::cerr << input["0"].size() << " to " << input["tx"].size() << std::endl;
    if(input["0"].size() == 0 && input["tx"].size() == 0) {
        return raft::stop;
    }

    if(input["0"].size() == 0 || input["tx"].size() == 0)
        return raft::proceed;

    input["0"].pop(img_in);

    cv::UMat tx;
    input["tx"].pop(tx);

    MetadataEnvelope<cv::UMat> out(img_in.Metadata());

    auto itx = tx.inv(cv::DECOMP_SVD);
    cv::warpAffine(img_in, out, itx.rowRange(0, 2), destSize, flags, borderMode, borderValue);

    output["0"].push(out);

    return raft::proceed;
}
