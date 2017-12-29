#include <RaftOCV.h>

int main(int argc, char** argv) {
    VideoCaptureSource src(argv[1]);

    DisplayFrameSink sink("Blur");
    raft::map m;

    ApplyFilterFilter blur(cv::getGaussianKernel(31, -1));

    m += src >> blur >> sink;

    m.exe();

    return( EXIT_SUCCESS );
}
