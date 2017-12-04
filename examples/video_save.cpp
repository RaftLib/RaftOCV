#include <raft>
#include <RaftOCV/imgproc/ConvertColorFilter.h>
#include <RaftOCV/videoio/VideoCaptureSink.h>
#include <opencv2/videoio/videoio_c.h>

#include "RaftOCV/videoio/UVCSource.h"

int main(int argc, const char** argv) {
    const char* videoSrc = "/dev/video0";
    if(argc > 1)
        videoSrc = argv[1];

    UVCSource src(videoSrc);
    ConvertColorFilter convert(CV_YUV2BGR_YUY2);
    VideoCaptureSink sink("my-file.avi", CV_FOURCC('M', 'J', 'P', 'G'));

    raft::map m;
    m += src >> convert >> sink;

    m.exe();
    return( EXIT_SUCCESS );
}

