//
// Created by justin on 3/12/17.
//

#include <opencv2/opencv.hpp>
#include "FindChessboardCornersKernel.h"
#include "Metadata.h"

FindChessboardCornersKernel::FindChessboardCornersKernel(const cv::Size &patternSize) : patternSize(patternSize) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<std::vector<cv::Point2f>>>("0");
    if(enableOutput) {
        output.addPort<MetadataEnvelope<cv::Mat>>("debug");
    }
}

raft::kstatus FindChessboardCornersKernel::run() {
    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    MetadataEnvelope<std::vector<cv::Point2f>> out(img_in.Metadata());
    bool success = cv::findChessboardCorners(img_in, patternSize, out, cv::CALIB_CB_FAST_CHECK);

    if(success) {
        cv::Mat gray;
        cv::cvtColor(img_in, gray, CV_RGB2GRAY);

        cv::cornerSubPix(gray, out, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS,
                                          30, 0.01));
    }

    if(enableOutput) {
        auto &debug = output["debug"].template allocate<MetadataEnvelope<cv::Mat>>(img_in.Metadata());
        debug = img_in.clone();
        cv::drawChessboardCorners(debug, patternSize, out, success);
        output["debug"].send();
    }

    input["0"].recycle( input["0"].size() );

    if(success)
        output["0"].push(out);


    return raft::proceed;
}
