
#include <opencv2/opencv.hpp>
#include "StereoCalibrateKernel.h"
#include "Metadata.h"

raft::kstatus StereoCalibrateKernel::run() {
    auto &points = input["0"].template peek<input_t>();

    imagePoints1.push_back(points.first);
    imagePoints2.push_back(points.second);
    matchedObjectPoints.push_back(objectPoints);

    std::cerr << "Got " << points.Metadata().originId << std::endl;
    input["0"].unpeek();
    input["0"].recycle();

    calculate();

    return raft::proceed;
}

StereoCalibrateKernel::StereoCalibrateKernel(cv::Size imageSize, const std::vector<cv::Point3f> &objectPoints) :
        imageSize(imageSize),
        objectPoints(objectPoints) {
    setupPorts();
}

void StereoCalibrateKernel::calculate() {
    using namespace cv;

    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = initCameraMatrix2D(matchedObjectPoints,imagePoints1,imageSize);
    cameraMatrix[1] = initCameraMatrix2D(matchedObjectPoints,imagePoints2,imageSize);
    Mat R, T, E, F;

    auto rms = cv::stereoCalibrate(matchedObjectPoints, imagePoints1, imagePoints2,
                        cameraMatrix[0], distCoeffs[0],
                        cameraMatrix[1], distCoeffs[1],
                        imageSize, R, T, E, F,
                        CALIB_FIX_ASPECT_RATIO +
                        CALIB_ZERO_TANGENT_DIST +
                        CALIB_USE_INTRINSIC_GUESS +
                        CALIB_SAME_FOCAL_LENGTH +
                        CALIB_RATIONAL_MODEL +
                        CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
                        TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5));

    std::cerr << cameraMatrix[0] << std::endl;
    std::cerr << cameraMatrix[1] << std::endl;

    std::cerr << R << std::endl;
    std::cerr << T << std::endl;
    std::cerr << E << std::endl;
    std::cerr << F << std::endl << std::endl << rms << std::endl << std::endl;
}

StereoCalibrateKernel::StereoCalibrateKernel(cv::Size imageSize, cv::Size boardSize, float squareSize) : imageSize(imageSize) {
    setupPorts();
    for(size_t j = 0; j < boardSize.height; j++ )
        for(size_t k = 0; k < boardSize.width; k++ )
            objectPoints.push_back(cv::Point3f(k*squareSize, j*squareSize, 0));

}

StereoCalibrateKernel::StereoCalibrateKernel() {
    setupPorts();
}

void StereoCalibrateKernel::setupPorts() {
    input.addPort<input_t>("0");
}
