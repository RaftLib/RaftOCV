#include "DenseOpticalFlow.h"
#include <src/RaftOCV/utility/Metadata.h>
#include <opencv/cv.hpp>
#include <opencv2/cudaoptflow.hpp>
#include <opencv2/opencv.hpp>

class DenseOpticalFlow_p {
    DenseOpticalFlow& self;
    cv::UMat prev;
    cv::cuda::GpuMat gpu_prev;
    size_t drop_cnt = 0;

    cv::Ptr<cv::cuda::FarnebackOpticalFlow> cudaFlow = cv::cuda::FarnebackOpticalFlow::create();
    cv::Ptr<cv::cuda::Filter> filter;

public:
    DenseOpticalFlow_p(DenseOpticalFlow &self) : self(self) {}

    raft::kstatus runCuda() {
        MetadataEnvelope<cv::UMat> img_in;
        self.input["0"].pop(img_in);

        if(drop_cnt++ < self.m_Skip) {
            return raft::proceed;
        }
        drop_cnt = 0;

        cv::UMat gray;
        cv::cvtColor(img_in, gray, cv::COLOR_RGB2GRAY);


        //cv::GaussianBlur(gray, gray, cv::Size(31,31), 0);

        if(gpu_prev.data == nullptr) {
            gpu_prev.upload(gray);
            filter = cv::cuda::createGaussianFilter(CV_8U, CV_8U, cv::Size(31, 31), 0);
            return raft::proceed;
        }

        MetadataEnvelope<cv::UMat> out(img_in.Metadata());
        cv::cuda::GpuMat gpu_denseFlow;
        cv::cuda::GpuMat gpu_gray;
        cv::Mat denseFlow;
        gpu_gray.upload(gray);
        filter->apply(gpu_gray, gpu_gray);

        cudaFlow->calc(gpu_prev, gpu_gray, gpu_denseFlow);

        gpu_denseFlow.download(denseFlow);

        cv::Mat outs[3];
        cv::Mat outf;
        cv::split(denseFlow, outs);
        outs[2] = outs[0].mul(outs[1]);
        cv::merge(outs, 3, outf);
        outf = outf * 100;
        outf.convertTo(out, CV_8UC3);

        cv::UMat magnitude, angle;
        cv::cartToPolar(outs[0], outs[1], magnitude, angle, true);

        //translate magnitude to range [0;1]
        double mag_max;
        cv::minMaxLoc(magnitude, 0, &mag_max);
        //magnitude.convertTo(magnitude, -1, 1.0 / mag_max);

        //build hsv image
        cv::Mat _hsv[3], hsv;
        _hsv[0] = angle.getMat(cv::ACCESS_READ);;
        _hsv[1] = cv::Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magnitude.getMat(cv::ACCESS_READ).clone();
        cv::merge(_hsv, 3, hsv);

        //convert to BGR and show
        cv::UMat bgr;//CV_32FC3 matrix
        cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);

        cv::UMat res;
        int optMag = 1;
        cv::threshold(magnitude, res, optMag,100, CV_THRESH_BINARY);

        cv::UMat res2;
        res.convertTo(res2, CV_8UC1, 255);

        // Create a structuring element (SE) and do some morphological operation in order to close holes, get unified connected components
        int morph_size = 2;
        cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                    cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                                                    cv::Point(morph_size, morph_size));
        morphologyEx(res2, res2, cv::MORPH_CLOSE, element, cv::Point(-1, -1), 12);
        //morphologyEx(src, src, MORPH_ERODE, element, Point(-1, -1), 8);


        gpu_prev.swap(gpu_gray);

        self.output["0"].push(bgr);
        return raft::proceed;
    }

    raft::kstatus run() {
        if(cudaFlow) {
            return runCuda();
        }

        MetadataEnvelope<cv::UMat> img_in;
        self.input["0"].pop(img_in);

        cv::UMat gray;
        cv::cvtColor(img_in, gray, cv::COLOR_RGB2GRAY);

        if(prev.empty()) {
            prev = gray;
            return raft::proceed;
        }

        MetadataEnvelope<cv::UMat> out(img_in.Metadata());
        cv::UMat denseFlow;
        cv::calcOpticalFlowFarneback(prev, gray, denseFlow, 0.5, 1,
                                     11, 2, 5, 1.2, 0);


        cv::Mat outs[3];
        cv::Mat outf;
        cv::split(denseFlow.getMat(cv::ACCESS_READ), outs);
        outs[2] = outs[0].mul(outs[1]);
        cv::merge(outs, 3, outf);
        outf = outf * 100;
        outf.convertTo(out, CV_8UC3);

        prev = gray;

        self.output["0"].push(out);
        return raft::proceed;
    }
};

raft::kstatus DenseOpticalFlow::run() {
    return p->run();
}

DenseOpticalFlow::DenseOpticalFlow() : p(new DenseOpticalFlow_p(*this)) {
    input.addPort<MetadataEnvelope<cv::UMat>>("0");
    output.addPort<MetadataEnvelope<cv::UMat>>("0");
}

DenseOpticalFlow::~DenseOpticalFlow() {

}
