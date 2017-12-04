
#include <opencv2/opencv.hpp>
#include "StereoCalibrateKernel.h"
#include "src/RaftOCV/utility/Metadata.h"

void StereoCalibrateKernel::OnKey(int key) {
    if(key == ' ' && !lastInput.first.empty()) {
        std::lock_guard<std::mutex> l(lastInputMutex);
        imagePoints.push_back(lastInput.first);
        otherImagePoints.push_back(lastInput.second);
        matchedObjectPoints.push_back(objectPoints);

        calculate();
    }
}

raft::kstatus StereoCalibrateKernel::run() {
    auto &points = input["0"].template peek<input_t>();

    {
        std::lock_guard<std::mutex> l(lastInputMutex);
        lastInput.swap(points);
    }


    std::cerr << "Got " << points.Metadata().originId << std::endl;
    input["0"].unpeek();
    input["0"].recycle();

    return raft::proceed;
}

void StereoCalibrateKernel::calculate() {
    using namespace cv;

    auto results = std::make_shared<StereoCalibrationResults>();

    results->right.cameraMatrix = initCameraMatrix2D(matchedObjectPoints, imagePoints, imageSize);
    results->left.cameraMatrix = initCameraMatrix2D(matchedObjectPoints, otherImagePoints, imageSize);

    results->rms = cv::stereoCalibrate(matchedObjectPoints, imagePoints, otherImagePoints,
                                       results->right.cameraMatrix, results->right.distCoeffs,
                                       results->left.cameraMatrix, results->left.distCoeffs,
                                       imageSize, results->rvecs, results->tvecs, results->E, results->F,
                                       CALIB_FIX_ASPECT_RATIO +
                                       CALIB_ZERO_TANGENT_DIST +
                                       CALIB_USE_INTRINSIC_GUESS +
                                       CALIB_SAME_FOCAL_LENGTH +
                                       CALIB_RATIONAL_MODEL +
                                       CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
                                       TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));

    results->imageSize = imageSize;
    cv::stereoRectify(results->right.cameraMatrix, results->right.distCoeffs,
                      results->left.cameraMatrix, results->left.distCoeffs,
                      imageSize,
                      results->rvecs, results->tvecs,
                      results->right.R, results->left.R,
                      results->right.P, results->left.P,
                      results->Q,
                      CALIB_ZERO_DISPARITY, 1, results->imageSize, &results->right.validRoi, &results->left.validRoi);

    std::cerr << results->right.validRoi << std::endl;
    std::cerr << results->left.validRoi << std::endl;
    std::cerr << results->rms << std::endl << std::endl;

    if(!saveFile.empty()) {
        results->Write(saveFile);
    }

    output["0"].push(results);
}

StereoCalibrateKernel::StereoCalibrateKernel(cv::Size imageSize, cv::Size boardSize, float squareSize) : imageSize(
        imageSize) {
    setupPorts();
    for (size_t j = 0; j < boardSize.height; j++)
        for (size_t k = 0; k < boardSize.width; k++)
            objectPoints.push_back(cv::Point3f(k * squareSize, j * squareSize, 0));

}

StereoCalibrateKernel::StereoCalibrateKernel(cv::Size imageSize, const std::vector<cv::Point3f> &objectPoints) :
        imageSize(imageSize),
        objectPoints(objectPoints) {
    setupPorts();
}

StereoCalibrateKernel::StereoCalibrateKernel() {
    setupPorts();
}

void StereoCalibrateKernel::setupPorts() {
    input.addPort<input_t>("0");
    output.addPort<output_t>("0");
}
