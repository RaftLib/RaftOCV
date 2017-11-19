#pragma once
#include <raft>
#include <opencv2/core/types.hpp>
#include "utility/Metadata.h"
#include "calib3d/CalibrateKernel.h"
#include "calib3d/CalibrationTypes.h"
#include "ui/KeyListener.h"

class StereoCalibrateKernel : public CalibrationBase, public KeyListener_t {
    std::vector<cv::Point3f> objectPoints;
    cv::Size imageSize;

    typedef std::pair<
            std::vector<cv::Point2f>,
            std::vector<cv::Point2f> > ImagePoints_t;

    typedef MetadataEnvelope<ImagePoints_t> input_t;
    std::mutex lastInputMutex;
    ImagePoints_t lastInput;

    std::vector<std::vector<cv::Point2f>> otherImagePoints;

    void calculate();
    void setupPorts();
public:
    std::string saveFile;
    typedef std::shared_ptr<CalibrationResults> output_t;

    template <typename... Args>
    StereoCalibrateKernel(Args&&... args) : CalibrationBase(std::forward<Args>(args)...){ setupPorts(); }

    StereoCalibrateKernel();
    StereoCalibrateKernel(cv::Size imageSize, cv::Size gridSize, float boardSize = 0.0254);
    StereoCalibrateKernel(cv::Size imageSize, const std::vector<cv::Point3f> &objectPoints);

    raft::kstatus run() override;

    void OnKey(int key) override;
};


