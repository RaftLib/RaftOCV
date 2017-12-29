#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include "src/RaftOCV/utility/Metadata.h"
#include "BgModel.h"

raft::kstatus BgModel::run() {
    MetadataEnvelope<cv::UMat> img_in;
    input["0"].pop(img_in);

    if(sum.empty()) {
        sum = cv::UMat::zeros(img_in.rows, img_in.cols, CV_32FC3);
        sum2 = cv::UMat::zeros(img_in.rows, img_in.cols, CV_32FC3);
    }

    cv::UMat img_inf;
    img_in.convertTo(img_inf, CV_32FC3);

    cv::add(sum, img_inf, sum, cv::UMat(), CV_32FC3);
    cv::add(sum2, img_inf.mul(img_inf), sum2, cv::UMat(), CV_32FC3);

    //total = total + cv::UMat(img_in.rows, img_in.cols, CV_32S, cv::Scalar(1));
    total += 1;

    return raft::proceed;
}

BgModel::BgModel() {
    input.addPort<MetadataEnvelope<cv::UMat>>("0");
}

cv::UMat BgModel::Mean() const {
    auto meanf = MeanF();
    cv::UMat mean;

    meanf.convertTo(mean, CV_8UC3);

    return mean;
}

cv::UMat BgModel::StdDev() const {
    auto meanf = MeanF().getMat(cv::ACCESS_READ);

    cv::Mat stddevf;
    cv::Mat stddev2 = (sum2.getMat(cv::ACCESS_READ) / (double)total) - (meanf.mul(meanf));
    cv::threshold(stddev2, stddev2, 0, 0, CV_THRESH_TOZERO);
    stddev2 = stddev2 + cv::Scalar(10,10,10);
    //cv::sqrt(stddev2, stddevf);
    double minVal, maxVal;
    cv::minMaxIdx(stddev2, &minVal, &maxVal);
    return stddev2.getUMat(cv::ACCESS_READ);
}

cv::UMat BgModel::MeanF() const {
    return cv::Mat(sum.getMat(cv::ACCESS_READ) / (double)total).getUMat(cv::ACCESS_READ);
}

raft::kstatus BgRemoval::run() {
    MetadataEnvelope<cv::UMat> img_in;
    input["0"].pop(img_in);

    cv::UMat imgf;
    img_in.convertTo(imgf, CV_32FC3);

    cv::absdiff(imgf, Mean, imgf);

    imgf = imgf.mul(imgf);
    cv::divide(imgf, StdDev, imgf);

    cv::Mat imgfs[3];
    cv::split(imgf.getMat(cv::ACCESS_READ), imgfs);

    double thresh = 3.0;
    cv::Mat maskf = imgfs[0].mul(imgfs[1]).mul(imgfs[2]);
    cv::GaussianBlur(maskf, maskf, cv::Size(21,21), 0);
    cv::threshold(maskf, maskf, thresh, 1., cv::THRESH_BINARY);
    cv::UMat mask;
    maskf.convertTo(mask, CV_8U);

    cv::UMat out;
    img_in.copyTo(out, mask);

    output["0"].push(out);

    return raft::proceed;
/*
    for(size_t i = 0;i < 3;i++) {
        cv::threshold(imgfs[i], imgfs[i], thresh, 1., cv::THRESH_BINARY);
    }

    cv::UMat maskf =
    cv::UMat mask;
    maskf.convertTo(mask, CV_8U);

    cv::UMat out;


    //output["0"].push(imgf);

    return raft::proceed;
    */
}

BgRemoval::BgRemoval(const cv::UMat &Mean, const cv::UMat &StdDev) : Mean(Mean), StdDev(StdDev) {
    input.addPort<MetadataEnvelope<cv::UMat>>("0");
    output.addPort<MetadataEnvelope<cv::UMat>>("0");
}
