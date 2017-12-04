#include <RaftOCV/ui/DisplayFrameSink.h>
#include <RaftOCV/imgproc/ConvertColorFilter.h>
#include <RaftOCV/videoio/VideoCaptureSource.h>
#include <RaftOCV/tracking/PoseEstimator.h>
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