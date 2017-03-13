#pragma once
#include <raft>
#include <opencv2/core/types.hpp>
#include "Metadata.h"

class StereoCalibrateKernel : public raft::kernel {
    std::vector<cv::Point3f> objectPoints;
    cv::Size imageSize;

    typedef std::pair<
            std::vector<cv::Point2f>,
            std::vector<cv::Point2f> > ImagePoints_t;

    typedef MetadataEnvelope<ImagePoints_t> input_t;

    std::vector<std::vector<cv::Point2f>> imagePoints1, imagePoints2;
    std::vector<std::vector<cv::Point3f> > matchedObjectPoints;

    void calculate();
    void setupPorts();
public:
    StereoCalibrateKernel();
    StereoCalibrateKernel(cv::Size imageSize, cv::Size gridSize, float boardSize = 0.0254);
    StereoCalibrateKernel(cv::Size imageSize, const std::vector<cv::Point3f> &objectPoints);

    raft::kstatus run() override;
};


