#pragma once


#include <RaftOCV.h>
#include <RaftOCV/utility/FunctorFilter.h>

class ApplyFilterFunctor {
    cv::UMat filter;
public:
    MetadataEnvelope<cv::UMat> operator()(const MetadataEnvelope<cv::UMat>& in);

    ApplyFilterFunctor(const cv::UMat &filter);
    ApplyFilterFunctor(const cv::Mat &filter);
};

typedef FunctorFilter<ApplyFilterFunctor> ApplyFilterFilter;


