#pragma once


#include <RaftOCV.h>
#include <RaftOCV/utility/FunctorFilter.h>

class ApplyFilterFunctor {
    cv::Mat filter;

    MetadataEnvelope<cv::Mat> operator()(const MetadataEnvelope<cv::Mat>& in);
};

typedef FunctorFilter<ApplyFilterFunctor> ApplyFilterFilter;


