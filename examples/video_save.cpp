#include <raft>
#include <ConvertColorFilter.h>
#include <VideoCaptureSink.h>
#include <opencv2/videoio/videoio_c.h>

#include "UVCSource.h"

int main() {
    UVCSource src;
    ConvertColorFilter convert(CV_YUV2BGR_YUY2);
    VideoCaptureSink sink("my-file.avi", CV_FOURCC('M', 'J', 'P', 'G'));

    raft::map m;
    m += src >> convert >> sink;

    m.exe();
    return( EXIT_SUCCESS );
}

