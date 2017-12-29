#pragma once

#include <raft>
#include <RaftOCV/utility/FunctorFilter.h>

class CannyEdgeFunctor {
 public:
    double threshold1;
    double threshold2;
    int apertureSize = 3;
    bool L2gradient = false;

    CannyEdgeFunctor(double threshold1 = 100, double threshold2 = 100, int apertureSize = 3, bool L2gradient = false);
    MetadataEnvelope<cv::UMat> operator()(const MetadataEnvelope<cv::UMat>& img_in);
};

typedef FunctorFilter<CannyEdgeFunctor> CannyEdgeKernel;