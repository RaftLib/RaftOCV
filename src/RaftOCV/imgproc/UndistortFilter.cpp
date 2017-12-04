#include <opencv2/imgproc.hpp>
#include <src/RaftOCV/utility/Metadata.h>
#include <src/RaftOCV/calib3d/CalibrateKernel.h>
#include "UndistortFilter.h"
#include "src/RaftOCV/stereo/StereoCalibrateKernel.h"

raft::kstatus UndistortFilter::run() {
    if(expectCalibKernel && input["calib"].size()) {
        auto size = input["calib"].size();
        if(size > 1)
            input["calib"].recycle(size - 1);

        std::shared_ptr<CalibrationResults> calib;
        input["calib"].pop(calib);

        if(auto c = std::dynamic_pointer_cast<SingleCalibrationCameraResults>(calib))
            calculate(*c);
        else if(auto c = std::dynamic_pointer_cast<StereoCalibrationResults>(calib))
            calculate(*c);
    }

    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    if(!map1.empty() && !map2.empty()) {
        auto &out = output["0"].template allocate<MetadataEnvelope<cv::Mat>>(img_in.Metadata());
        cv::remap(img_in, out, map1, map2, cv::INTER_LINEAR);
        output["0"].send();
    } else {
        auto &out = output["0"].template allocate<MetadataEnvelope<cv::Mat>>(img_in.Metadata());
        out = img_in;
        output["0"].send();
    }

    return (raft::proceed);
}

UndistortFilter::UndistortFilter(const cv::Mat &map1, const cv::Mat &map2) : map1(map1), map2(map2), expectCalibKernel(false) {
    setupPorts();
}

UndistortFilter::UndistortFilter(const SingleCalibrationCameraResults &calib) : expectCalibKernel(false) {
    setupPorts();
    calculate(calib);
}

UndistortFilter::UndistortFilter(bool isRightCamera, const StereoCalibrationResults &calib) : isStereo(true),
                                                                                              isRightCamera(isRightCamera),
                                                                                              expectCalibKernel(false) {
    setupPorts();
    calculate(calib);
}
void UndistortFilter::calculate(const SingleCalibrationCameraResults &calib) {
    cv::Mat R, newCamMat;
    cv::initUndistortRectifyMap(calib.cameraMatrix, calib.distCoeffs, R, newCamMat, calib.imageSize, CV_16SC2, map1, map2);
}

void UndistortFilter::calculate(const StereoCalibrationResults &calib) {
    auto& cam = isRightCamera ? calib.right : calib.left;
    cv::Mat P = cam.cameraMatrix;
    if(!cam.cameraMatrix.empty() && !cam.distCoeffs.empty())
        cv::initUndistortRectifyMap(cam.cameraMatrix, cam.distCoeffs, cam.R, P, calib.imageSize, CV_16SC2, map1, map2);
    std::cerr << cam.cameraMatrix << std::endl;
    std::cerr << cam.distCoeffs << std::endl;
    std::cerr << cam.R << std::endl;
    std::cerr << cam.P << std::endl;
}

UndistortFilter::UndistortFilter() : expectCalibKernel(true){
    setupPorts();
}

UndistortFilter::UndistortFilter(bool isRightCamera) : isStereo(true), isRightCamera(isRightCamera){
    setupPorts();
}

void UndistortFilter::setupPorts() {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    if(expectCalibKernel)
        input.addPort<std::shared_ptr<CalibrationResults>>("calib");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}

