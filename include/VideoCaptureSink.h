#pragma once

#include <raft>

struct VideoWriterSink_p;
class VideoCaptureSink : public raft::kernel{
    std::unique_ptr<VideoWriterSink_p> p;
    const std::string filename;
    int fourcc;
    double fps = 30;
    bool isColor=true;

public:
    raft::kstatus run() override;
    ~VideoCaptureSink();
    VideoCaptureSink(const std::string &filename, int fourcc, double fps = 30, bool isColor = true);
};


