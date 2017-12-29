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
    fs << (prefix + "cameraMatrix") << cameraMatrix.getMat(cv::ACCESS_READ);
    fs << (prefix + "distCoeffs") << distCoeffs.getMat(cv::ACCESS_READ);
    fs << (prefix + "validRoi") << validRoi;
    fs << (prefix + "P") << P.getMat(cv::ACCESS_READ);
    fs << (prefix + "R") << R.getMat(cv::ACCESS_READ);
}

static cv::UMat read_from_file(cv::FileStorage &fs, const std::string& field) {
    cv::Mat tmp;
    fs[field] >> tmp;
    return tmp.getUMat(cv::ACCESS_READ);
}

void SingleCameraCalibration::Read(cv::FileStorage &fs, const std::string &n) {
    std::string prefix = "";
    if(!n.empty())
        prefix = n + "_";

    cv::Mat camera, dist, _P, _R;
    cameraMatrix = read_from_file(fs, prefix + "cameraMatrix");
    distCoeffs = read_from_file(fs, prefix + "distCoeffs");
    fs[prefix + "validRoi"] >> validRoi;
    P = read_from_file(fs, prefix + "P");
    R = read_from_file(fs, prefix + "R");
}


void StereoCalibrationResults::Write(cv::FileStorage &fs) {
    fs << "ImageSize" << this->imageSize;
    fs << "E" << this->E.getMat(cv::ACCESS_READ);
    fs << "F" << this->F.getMat(cv::ACCESS_READ);
    fs << "Q" << this->Q.getMat(cv::ACCESS_READ);
    fs << "rms" << this->rms;

    right.Write(fs, "right");
    left.Write(fs, "left");
}


void StereoCalibrationResults::Read(cv::FileStorage &fs) {
    fs["ImageSize"] >> this->imageSize;
    this->E = read_from_file(fs, "E");
    this->F = read_from_file(fs, "F");
    this->Q = read_from_file(fs, "Q");
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
