#pragma once
#include <raft>
#include <opencv2/core/mat.hpp>
#include <QtCore/QObject>
#include <RaftOCV/utility/Properties.h>

struct VideoCapture_p;
class VideoCaptureSource : public QObject , public raft::kernel{
    Q_OBJECT;

    std::unique_ptr<VideoCapture_p> p;

public:
    int frameCap = -1;
    bool getNextFrame(cv::Mat& img);
    bool isRealTime = false;


    DECLARE_PROPERTY(bool, IsRealTime) = false;

    cv::Size frameSize;

    ~VideoCaptureSource();

    VideoCaptureSource(VideoCapture_p* p);
    VideoCaptureSource();

    VideoCaptureSource(const cv::String &filename);

    VideoCaptureSource(const cv::String &filename, int apiPreference);

    VideoCaptureSource(int index);


    raft::kstatus run() override;
};


