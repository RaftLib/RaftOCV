#include <opencv/cv.hpp>
#include "CalibrateKernel.h"

CalibrationBase::CalibrationBase(cv::Size imageSize, cv::Size boardSize, float squareSize) : imageSize(imageSize) {
    for(size_t j = 0; j < boardSize.height; j++ )
        for(size_t k = 0; k < boardSize.width; k++ )
            objectPoints.push_back(cv::Point3f(k*squareSize, j*squareSize, 0));

}

CalibrationBase::CalibrationBase(cv::Size imageSize, const std::vector<cv::Point3f> &objectPoints) :
        imageSize(imageSize),
        objectPoints(objectPoints) {

}

CalibrationBase::CalibrationBase() {

}

void CalibrateKernel::setupPorts() {
    input.addPort<input_t>("0");
    output.addPort<SingleCalibrationCameraResults>("0");
}

raft::kstatus CalibrateKernel::run() {
    input_t points;
    input["0"].pop(points);

    imagePoints.push_back(points);
    matchedObjectPoints.push_back(objectPoints);

    std::cerr << "Got " << points.Metadata().originId << std::endl;

    calculate();

    return raft::proceed;
}

void CalibrateKernel::calculate() {
    using namespace cv;

    SingleCalibrationCameraResults results;
    results.cameraMatrix = initCameraMatrix2D(matchedObjectPoints,imagePoints,imageSize).getUMat(cv::ACCESS_READ);
    results.rms = cv::calibrateCamera(matchedObjectPoints, imagePoints, imageSize,
                                      results.cameraMatrix, results.distCoeffs, results.rvecs, results.tvecs,
                        CALIB_FIX_ASPECT_RATIO +
                        CALIB_ZERO_TANGENT_DIST +
                        CALIB_USE_INTRINSIC_GUESS +
                        CALIB_SAME_FOCAL_LENGTH +
                        CALIB_RATIONAL_MODEL +
                        CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
                        TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5)
    );
    results.imageSize = imageSize;

    if(results.rms > 10) {
        matchedObjectPoints.pop_back();
        imagePoints.pop_back();
    } else {
        output["0"].push(results);
    }
}
