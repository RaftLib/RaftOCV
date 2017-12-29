#include "ApplyFilter.h"

MetadataEnvelope<cv::UMat> ApplyFilterFunctor::operator()(const MetadataEnvelope<cv::UMat> &in) {
    MetadataEnvelope<cv::UMat> rtn(in);
    cv::filter2D(in, rtn, -1, filter);
    return rtn;
}

ApplyFilterFunctor::ApplyFilterFunctor(const cv::UMat &filter) : filter(filter) {}
