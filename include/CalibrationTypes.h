#pragma once

#include <iosfwd>
#include <ostream>
#include <opencv2/core/types.hpp>
#include <opencv2/core/persistence.hpp>

struct SingleCameraCalibration {
    cv::Mat cameraMatrix, distCoeffs;
    cv::Mat R, P;
    cv::Rect validRoi;
    friend std::ostream &operator<<(std::ostream &os, const SingleCameraCalibration &calibration);
    void Write(cv::FileStorage& fs, const std::string& n);

    void Read(cv::FileStorage &storage, const std::string &prefix);
};

struct CalibrationResults {
    virtual ~CalibrationResults() {}
    double rms = 0;
    cv::Mat rvecs, tvecs;
    cv::Size imageSize;
    friend std::ostream &operator<<(std::ostream &os, const CalibrationResults &results);
};

struct SingleCalibrationCameraResults : public CalibrationResults, public SingleCameraCalibration {
    friend std::ostream &operator<<(std::ostream &os, const SingleCalibrationCameraResults &results);
};

struct StereoCalibrationResults : public CalibrationResults {
    SingleCameraCalibration right, left;
    cv::Mat E, F, Q;
    StereoCalibrationResults();
    StereoCalibrationResults(const std::string& fn);
    void Read(cv::FileStorage &fs);
    void Write(const std::string& fn);
    void Write(cv::FileStorage& fs);
};
