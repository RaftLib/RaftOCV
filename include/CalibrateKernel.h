#pragma once

#include <raft>
#include <opencv2/core/types.hpp>
#include <Metadata.h>
#include <ostream>
#include "CalibrationTypes.h"

class CalibrationBase : public raft::kernel {
protected:
    std::vector<cv::Point3f> objectPoints;
    cv::Size imageSize;

    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<std::vector<cv::Point3f> > matchedObjectPoints;

public:
    CalibrationBase();
    CalibrationBase(cv::Size imageSize, cv::Size gridSize, float boardSize = 0.0254);
    CalibrationBase(cv::Size imageSize, const std::vector<cv::Point3f> &objectPoints);
};

class CalibrateKernel : public CalibrationBase {
public:
    typedef std::vector<cv::Point2f> ImagePoints_t;
    typedef MetadataEnvelope<ImagePoints_t> input_t;
    SingleCalibrationCameraResults previousResults;

    void setupPorts();
    void calculate();
public:
    template <typename... Args>
    CalibrateKernel(Args&&... args) : CalibrationBase(std::forward<Args>(args)...){
        setupPorts();
    }
    raft::kstatus run() override;
};


