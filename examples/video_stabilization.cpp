#include <ui/DisplayFrameSink.h>
#include <raft>
#include <imgproc/ConvertColorFilter.h>
#include <videoio/VideoCaptureSource.h>
#include <tracking/PoseEstimator.h>
#include <tracking/StabalizeVideo.h>
#include <videoio/UVCSource.h>
#include <utility/VectorizeData.h>
#include <opencv/cv.hpp>
#include <videoio/VideoCaptureSink.h>
#include "imgproc/CannyEdgeKernel.h"
#include "../src/imgproc/DiffMask.h"
#include "../src/imgproc/WarpAffine.h"
#include "utility/FromVector.h"

static std::vector<cv::Mat> CenterTransforms(const std::vector<cv::Mat>& txs,
                                             cv::Size frameSize,
                                             cv::Size& targetSize) {

    float minx = 0, miny = 0,
            maxx = frameSize.width,
            maxy = frameSize.height;

    for(auto& tx : txs) {
        auto bounds = { cv::Vec3f(0, 0, 1),
                        cv::Vec3f(frameSize.width, 0, 1),
                        cv::Vec3f(0, frameSize.height, 1),
                        cv::Vec3f(frameSize.width, frameSize.height, 1)
        };
        for(auto pt : bounds) {
            std::cout << tx << "x" << pt << std::endl;
            auto npt = (cv::Vec3f)(cv::Mat)(tx.inv(cv::DECOMP_SVD) * cv::Mat(pt));
            std::cout << npt << std::endl;
            minx = std::min(minx, npt[0]);
            maxx = std::max(maxx, npt[0]);
            miny = std::min(miny, npt[1]);
            maxy = std::max(maxy, npt[1]);
        }
    }

    auto newTxs = txs;
    newTxs.insert(newTxs.begin(), cv::Mat_<float>::eye(3, 3));

    cv::Mat_<float> adjust = cv::Mat::eye(3,3, CV_32F);
    adjust(0, 2) = minx;
    adjust(1, 2) = miny;

    for(auto& newTx : newTxs) {
        std::cerr << adjust << std::endl;

        std::cerr << newTx << std::endl;
        newTx = newTx * adjust;
        std::cerr << newTx << std::endl << std::endl;
    }

    targetSize = cv::Size(maxx-minx, maxy-miny);
    return newTxs;
}

int main(int argc, char** argv) {
    cv::Size frameSize;
    VectorizeData<cv::Mat> txs;
    int frameCap = -1;
    {
        VideoCaptureSource src(argv[1]);
        DisplayFrameSink sink("Stabalize");
        raft::map m;

        src.frameCap = frameCap;

        HumanPoseEstimationKernel pose;
        CannyEdgeKernel canny;
        StabalizeVideo flow;
        DiffMask diff;

        m += src >> flow["0"].txOut() >> txs;
        m += flow["0"].videoOut() >> sink;

        m.exe();

        frameSize = src.frameSize;
    }

    cv::Size targetSize;
    auto newTxs = CenterTransforms(txs.Data(), frameSize, targetSize);

    {
        VideoCaptureSource src(argv[1]);
        VideoCaptureSink sink(std::string(argv[1]) + ".stabalized.avi", CV_FOURCC('M', 'J', 'P', 'G'));

        FromVector_<cv::Mat> txsSrc(newTxs);

        WarpAffine warp(targetSize, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0xff, 0x39, 0xec));

        raft::map m;

        src.frameCap = frameCap;

        m += src >> warp["0"];
        m += txsSrc >> warp["tx"] >> sink;

        m.exe();

        frameSize = src.frameSize;
    }


    return( EXIT_SUCCESS );
}