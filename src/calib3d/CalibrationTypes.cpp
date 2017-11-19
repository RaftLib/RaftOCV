#include "CalibrationTypes.h"
#include <opencv/cv.hpp>

std::ostream &operator<<(std::ostream &os, const SingleCalibrationCameraResults &results) {
    os << static_cast<const CalibrationResults &>(results) << ' '
       << static_cast<const SingleCameraCalibration &>(results);
    return os;
}

std::ostream &operator<<(std::ostream &os, const CalibrationResults &results) {
    os << "rms: " << results.rms << std::endl;
    os << "rvecs: " << results.rvecs << std::endl;
    os << "tvecs: " << results.tvecs << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const SingleCameraCalibration &calibration) {
    os << "cameraMatrix: " << calibration.cameraMatrix << std::endl;
    os << "distCoeffs: " << calibration.distCoeffs << std::endl;
    return os;
}

void SingleCameraCalibration::Write(cv::FileStorage &fs, const std::string &n) {
    std::string prefix = "";
    if(!n.empty())
        prefix = n + "_";
    fs << (prefix + "cameraMatrix") << cameraMatrix;
    fs << (prefix + "distCoeffs") << distCoeffs;
    fs << (prefix + "validRoi") << validRoi;
    fs << (prefix + "P") << P;
    fs << (prefix + "R") << R;
}

void SingleCameraCalibration::Read(cv::FileStorage &fs, const std::string &n) {
    std::string prefix = "";
    if(!n.empty())
        prefix = n + "_";
    fs[prefix + "cameraMatrix"] >> cameraMatrix;
    fs[prefix + "distCoeffs"] >> distCoeffs;
    fs[prefix + "validRoi"] >> validRoi;
    fs[prefix + "P"] >> P;
    fs[prefix + "R"] >> R;
}


void StereoCalibrationResults::Write(cv::FileStorage &fs) {
    fs << "ImageSize" << this->imageSize;
    fs << "E" << this->E;
    fs << "F" << this->F;
    fs << "Q" << this->Q;
    fs << "rms" << this->rms;

    right.Write(fs, "right");
    left.Write(fs, "left");
}


void StereoCalibrationResults::Read(cv::FileStorage &fs) {
    fs["ImageSize"] >> this->imageSize;
    fs["E"] >> this->E;
    fs["F"] >> this->F;
    fs["Q"] >> this->Q;
    fs["rms"] >> this->rms;

    right.Read(fs, "right");
    left.Read(fs, "left");
}

StereoCalibrationResults::StereoCalibrationResults() {

}

StereoCalibrationResults::StereoCalibrationResults(const std::string &fn) {
    cv::FileStorage fs(fn, cv::FileStorage::READ);
    Read(fs);
}

void StereoCalibrationResults::Write(const std::string &fn) {
    cv::FileStorage fs(fn, cv::FileStorage::WRITE);
    Write(fs);
}
