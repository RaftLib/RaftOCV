#pragma once


#include <RaftOCV.h>
#include <RaftOCV/utility/FunctorFilter.h>

class ApplyFilterFunctor {
    cv::Mat filter;
public:
    MetadataEnvelope<cv::Mat> operator()(const MetadataEnvelope<cv::Mat>& in);

    ApplyFilterFunctor(const cv::Mat &filter);
};

typedef FunctorFilter<ApplyFilterFunctor> ApplyFilterFilter;


