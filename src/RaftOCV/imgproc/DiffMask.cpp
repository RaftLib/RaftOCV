#include <opencv2/core.hpp>
#include <opencv/cv.hpp>
#include "DiffMask.h"
#include "src/RaftOCV/utility/Metadata.h"
#include <opencv2/cudabgsegm.hpp>

template <> struct PointerMember_<DiffMask> {
    cv::Ptr<cv::BackgroundSubtractor> bgSub;

    PointerMember_() : bgSub(cv::createBackgroundSubtractorMOG2(33, 16, false)) {}

    cv::Mat run(const cv::Mat& img) {
        cv::Mat mask, rtn;
        bgSub->apply(img, mask);

        img.copyTo(rtn, mask);
        return rtn;
    }
};

DiffMask::~DiffMask() {

}

raft::kstatus DiffMask::run() {
    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    MetadataEnvelope<cv::Mat> out(img_in.Metadata());

    out = p->run(img_in);
    output["0"].push(out);

    return raft::proceed;
}

DiffMask::DiffMask() : KernelTemplate(new PointerMember_<DiffMask>()){
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}
