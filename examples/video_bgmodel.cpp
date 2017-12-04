#include <raft>
#include <RaftOCV.h>
#include <opencv/cv.hpp>


int main(int argc, char** argv) {
    BgModel bg;
    {
        VideoCaptureSource src(argv[1]);
        raft::map m;

        m += src >> bg;

        m.exe();

        cv::imwrite(std::string(argv[1]) + ".mean.png", bg.Mean());

        cv::Mat stdDev;
        bg.StdDev().convertTo(stdDev, CV_8UC3);

        cv::imwrite(std::string(argv[1]) + ".stddev.png", stdDev);
    }

    {
        VideoCaptureSource src(argv[1]);
        BgRemoval bgr(bg.MeanF(), bg.StdDev());
        DisplayFrameSink sink("Removed BG");

        DiffMask diff;

        raft::map m;

        m += src >> bgr >> sink;

        m.exe();

    }

    return( EXIT_SUCCESS );
}