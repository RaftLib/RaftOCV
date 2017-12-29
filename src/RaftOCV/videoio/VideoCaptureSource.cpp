
#include <opencv2/videoio.hpp>
#include "VideoCaptureSource.h"
#include "src/RaftOCV/utility/Metadata.h"

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


bool VideoCaptureSource::getNextFrame(cv::UMat &img) {
    if(frameCap!=-1) {
        if(frameCap == 0 || frameCap-- == 0)
            return false;
    }
    bool success = p->read(img);

    if(success) {
        frameSize = cv::Size(img.cols, img.rows);
        if(p->isFileBased) {
            if (p->lastMs == -1) {
                p->lastMs = p->get(cv::CAP_PROP_POS_MSEC);
            } else {
                auto now = p->get(cv::CAP_PROP_POS_MSEC);
                if(isRealTime) {
                    std::this_thread::sleep_for(std::chrono::milliseconds((int) (now - p->lastMs)));
                }
                p->lastMs = now;
            }
        }
    }
    return success;
}

raft::kstatus VideoCaptureSource::run() {
    MetadataEnvelope<cv::UMat> frame;
    bool success = getNextFrame(frame);
    frame.metadata.originId = p->frameId++;

    if(success) {
        output["0"].push(frame);
    }
    return success ? raft::proceed : raft::stop;
}

VideoCaptureSource::~VideoCaptureSource() {

}

VideoCaptureSource::VideoCaptureSource(VideoCapture_p * p) : p(p){
    output.addPort<MetadataEnvelope<cv::UMat>>("0");
}

VideoCaptureSource::VideoCaptureSource() : VideoCaptureSource(new VideoCapture_p()){

}

VideoCaptureSource::VideoCaptureSource(const cv::String &filename) : VideoCaptureSource(new VideoCapture_p(filename)){

}

VideoCaptureSource::VideoCaptureSource(const cv::String &filename, int apiPreference) : VideoCaptureSource(new VideoCapture_p(filename, apiPreference)){

}

VideoCaptureSource::VideoCaptureSource(int index) : VideoCaptureSource(new VideoCapture_p(index)){

}

