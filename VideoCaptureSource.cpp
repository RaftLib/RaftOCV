
#include <opencv2/videoio.hpp>
#include "VideoCaptureSource.h"
#include "Metadata.h"

struct VideoCapture_p : public cv::VideoCapture {
    VideoCapture_p() {}

    VideoCapture_p(const cv::String &filename) : VideoCapture(filename) {
        isFileBased = true;
    }

    VideoCapture_p(const cv::String &filename, int apiPreference) : VideoCapture(filename, apiPreference) {
        isFileBased = true;
    }

    VideoCapture_p(int index) : VideoCapture(index) {}

    size_t frameId = 1;
    double lastMs = -1;
    bool isFileBased = false;
};

raft::kstatus VideoCaptureSource::run() {
    MetadataEnvelope<cv::Mat> frame;
    bool success = p->read(frame);
    frame.metadata.originId = p->frameId++;

    if(success) {
        if(p->isFileBased) {
            if (p->lastMs == -1) {
                p->lastMs = p->get(cv::CAP_PROP_POS_MSEC);
            } else {
                auto now = p->get(cv::CAP_PROP_POS_MSEC);
                std::this_thread::sleep_for(std::chrono::milliseconds((int) (now - p->lastMs)));
                p->lastMs = now;
            }
        }
        output["0"].push(frame);
    }
    return success ? raft::proceed : raft::stop;
}

VideoCaptureSource::~VideoCaptureSource() {

}

VideoCaptureSource::VideoCaptureSource(VideoCapture_p * p) : p(p){
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}

VideoCaptureSource::VideoCaptureSource() : VideoCaptureSource(new VideoCapture_p()){

}

VideoCaptureSource::VideoCaptureSource(const cv::String &filename) : VideoCaptureSource(new VideoCapture_p(filename)){

}

VideoCaptureSource::VideoCaptureSource(const cv::String &filename, int apiPreference) : VideoCaptureSource(new VideoCapture_p(filename, apiPreference)){

}

VideoCaptureSource::VideoCaptureSource(int index) : VideoCaptureSource(new VideoCapture_p(index)){

}
