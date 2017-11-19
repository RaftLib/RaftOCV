#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include "utility/Metadata.h"
#include "BgModel.h"

raft::kstatus BgModel::run() {
    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    if(!sum.data) {
        sum = cv::Mat::zeros(img_in.rows, img_in.cols, CV_32FC3);
        sum2 = cv::Mat::zeros(img_in.rows, img_in.cols, CV_32FC3);
    }

    cv::Mat img_inf;
    img_in.convertTo(img_inf, CV_32FC3);

    cv::add(sum, img_inf, sum, cv::Mat(), CV_32FC3);
    cv::add(sum2, img_inf.mul(img_inf), sum2, cv::Mat(), CV_32FC3);

    //total = total + cv::Mat(img_in.rows, img_in.cols, CV_32S, cv::Scalar(1));
    total += 1;

    return raft::proceed;
}

BgModel::BgModel() {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
}

cv::Mat BgModel::Mean() const {
    auto meanf = MeanF();
    cv::Mat mean;

    meanf.convertTo(mean, CV_8UC3);

    return mean;
}

cv::Mat BgModel::StdDev() const {
    auto meanf = MeanF();

    cv::Mat stddevf;
    cv::Mat stddev2 = (sum2 / (double)total) - (meanf.mul(meanf));
    cv::threshold(stddev2, stddev2, 0, 0, CV_THRESH_TOZERO);
    stddev2 = stddev2 + cv::Scalar(10,10,10);
    //cv::sqrt(stddev2, stddevf);
    double minVal, maxVal;
    cv::minMaxIdx(stddev2, &minVal, &maxVal);
    return stddev2;
}

cv::Mat BgModel::MeanF() const {
    return cv::Mat(sum / (double)total);
}

raft::kstatus BgRemoval::run() {
    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    cv::Mat imgf;
    img_in.convertTo(imgf, CV_32FC3);

    cv::absdiff(imgf, Mean, imgf);

    imgf = imgf.mul(imgf);
    cv::divide(imgf, StdDev, imgf);

    cv::Mat imgfs[3];
    cv::split(imgf, imgfs);

    double thresh = 3.0;
    cv::Mat maskf = imgfs[0].mul(imgfs[1]).mul(imgfs[2]);
    cv::GaussianBlur(maskf, maskf, cv::Size(21,21), 0);
    cv::threshold(maskf, maskf, thresh, 1., cv::THRESH_BINARY);
    cv::Mat mask;
    maskf.convertTo(mask, CV_8U);

    cv::Mat out;
    img_in.copyTo(out, mask);

    output["0"].push(out);

    return raft::proceed;
/*
    for(size_t i = 0;i < 3;i++) {
        cv::threshold(imgfs[i], imgfs[i], thresh, 1., cv::THRESH_BINARY);
    }

    cv::Mat maskf =
    cv::Mat mask;
    maskf.convertTo(mask, CV_8U);

    cv::Mat out;


    //output["0"].push(imgf);

    return raft::proceed;
    */
}

BgRemoval::BgRemoval(const cv::Mat &Mean, const cv::Mat &StdDev) : Mean(Mean), StdDev(StdDev) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}
