#pragma once
#include <raft>

struct VideoCapture_p;
class VideoCaptureSource : public raft::kernel {
    std::unique_ptr<VideoCapture_p> p;

public:
    ~VideoCaptureSource();

    VideoCaptureSource(VideoCapture_p* p);
    VideoCaptureSource();

    VideoCaptureSource(const cv::String &filename);

    VideoCaptureSource(const cv::String &filename, int apiPreference);

    VideoCaptureSource(int index);

    raft::kstatus run() override;
};


