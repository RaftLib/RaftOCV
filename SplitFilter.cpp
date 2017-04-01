#include <opencv2/core/mat.hpp>
#include "Metadata.h"
#include "SplitFilter.h"

raft::kstatus SplitFilter::run() {
    MetadataEnvelope<cv::Mat> in, outA, outB;
    input["0"].pop(in);

    outA = cv::Mat(in, cv::Rect(0, 0, in.size().width / 2, in.size().height));
    outB = cv::Mat(in, cv::Rect(in.size().width / 2, 0, in.size().width / 2, in.size().height));
    outA.metadata = in.Metadata();
    outB.metadata = in.Metadata();

    output["0"].push(outA);
    output["1"].push(outB);

    return raft::proceed;
}

SplitFilter::SplitFilter() {
    input.addPort<  MetadataEnvelope<cv::Mat> > ("0");
    output.addPort< MetadataEnvelope<cv::Mat> > ("0");
    output.addPort< MetadataEnvelope<cv::Mat> > ("1");
}
