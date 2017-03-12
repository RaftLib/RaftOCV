#include <opencv2/core/core.hpp>
#include "StereoBMFilter.h"

struct StereoBMFilter_p {
    StereoBMFilter& t;

    cv::Ptr<cv::StereoBM> sbm;
    cv::Mat cameraMatrix[2], distCoeffs[2];
    cv::Mat R1, R2, P1, P2, Q;
    cv::Mat rmap[2][2];
    cv::Size imageSize = { 0, 0 };
    cv::Mat depth;
    cv::Mat left, right;

    StereoBMFilter_p(StereoBMFilter &t) : t(t), sbm(cv::StereoBM::create(160, 21)) {
        sbm->setPreFilterSize(5);
        sbm->setPreFilterCap(25);
        sbm->setMinDisparity(1);
        sbm->setTextureThreshold(1);
        sbm->setUniquenessRatio(5);
        sbm->setSpeckleWindowSize(500);
        sbm->setSpeckleRange(20);

        cameraMatrix[0] = (cv::Mat_<double>(3,3) << 414.872943, 0, 316.062049, 0, 414.065879, 191.817217, 0, 0, 1);
        cameraMatrix[1] = (cv::Mat_<double>(3,3) << 415.322011, 0, 313.360348, 0, 416.459335, 181.660409, 0, 0, 1);

        distCoeffs[0] = (cv::Mat_<double>(1,5) << -0.008932, -0.071132, -0.007234, -0.006453, 0);
        distCoeffs[1] = (cv::Mat_<double>(1,5) << -0.010971, -0.077097, -0.007257, -0.010744, 0);

        R1 = (cv::Mat_<double>(3,3) << 0.998664, -0.000563, -0.051665, 0.000739, 0.999994, 0.003392, 0.051663, -0.003425, 0.998659);
        R2 = (cv::Mat_<double>(3,3) << 0.998153, -0.003609, -0.060645, 0.003402, 0.999988, -0.003515, 0.060657, 0.003302, 0.998153);

        P1 = cameraMatrix[0];
        P2 = cameraMatrix[1];
    }

    raft::kstatus run() {
        auto &in = t.input["0"].template peek<cv::Mat>();

        cv::Size s(in.cols / 2, in.rows);
        if(s != imageSize) {
            imageSize = s;

            cv::initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
            cv::initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
            depth = cv::Mat(s.height, s.width, CV_16S );

            left.create(imageSize, CV_8UC1);
            right.create(imageSize, CV_8UC1);
        }

        cv::Mat right_roi(in, cv::Rect(0, 0, s.width,s.height));
        cv::Mat left_roi(in, cv::Rect(s.width, 0, s.width,s.height));

        cv::cvtColor(left_roi, left, cv::COLOR_RGB2GRAY);
        cv::cvtColor(right_roi, right, cv::COLOR_RGB2GRAY);

        t.input["0"].unpeek();
        t.input["0"].recycle(1);

        cv::Mat img1r, img2r;
        remap(right, img1r, rmap[0][0], rmap[0][1], cv::INTER_LINEAR);
        remap(left, img2r, rmap[1][0], rmap[1][1], cv::INTER_LINEAR);
        right = img1r;
        left= img2r;

        sbm->compute(left, right, depth);

        auto &out = t.output["0"].template allocate<cv::Mat>();
        out = depth.clone();
        t.output["0"].send();

        return raft::proceed;
    }
};

StereoBMFilter::StereoBMFilter() : p(new StereoBMFilter_p(*this)) {
    input.addPort<cv::Mat>("0");
    output.addPort<cv::Mat>("0");
}

StereoBMFilter::~StereoBMFilter() {

}

raft::kstatus StereoBMFilter::run() {
    return p->run();
}
