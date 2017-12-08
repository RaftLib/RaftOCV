#include "ApplyFilter.h"

MetadataEnvelope<cv::Mat> ApplyFilterFunctor::operator()(const MetadataEnvelope<cv::Mat> &in) {
    MetadataEnvelope<cv::Mat> rtn(in);
    cv::filter2D(in, rtn, -1, filter);
    return rtn;
}

ApplyFilterFunctor::ApplyFilterFunctor(const cv::Mat &filter) : filter(filter) {}
