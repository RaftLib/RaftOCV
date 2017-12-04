
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include "VideoCaptureSink.h"
#include "src/RaftOCV/utility/Metadata.h"

struct VideoWriterSink_p : public cv::VideoWriter {
    VideoWriterSink_p() {}

    VideoWriterSink_p(const cv::String &filename, int fourcc, double fps, const cv::Size &frameSize, bool isColor)
            : VideoWriter(filename, fourcc, fps, frameSize, isColor) {}
};

VideoCaptureSink::VideoCaptureSink(const std::string &filename, int fourcc, double fps, bool isColor) : filename(
        filename), fourcc(fourcc), fps(fps), isColor(isColor) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
}

raft::kstatus VideoCaptureSink::run() {
    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    if(!p) {
        p.reset(new VideoWriterSink_p(filename, fourcc, fps, cv::Size(img_in.cols, img_in.rows), isColor));
    }

    p->write(img_in);

    return raft::proceed;
}

VideoCaptureSink::~VideoCaptureSink() {

}
