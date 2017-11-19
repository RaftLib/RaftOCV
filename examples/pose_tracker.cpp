#include <ui/DisplayFrameSink.h>
#include <imgproc/ConvertColorFilter.h>
#include <videoio/VideoCaptureSource.h>
#include <tracking/PoseEstimator.h>
#include <opencv/cv.hpp>

cv::Size targetSize;

int main(int argc, char** argv) {
    VideoCaptureSource src(argv[1]);
    DisplayFrameSink sink("Stabalize");
    raft::map m;

    HumanPoseEstimationKernel pose;

    m += src >> pose >> sink;

    m.exe();

    return( EXIT_SUCCESS );
}