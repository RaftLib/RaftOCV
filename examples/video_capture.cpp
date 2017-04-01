#include <raft>
#include <DisplayFrameSink.h>
#include <ConvertColorFilter.h>

#include "UVCSource.h"
#include "PointCloudFilter.h"

int main(int argc, const char** argv) {
    const char* videoSrc = "/dev/video0";
    if(argc > 1)
        videoSrc = argv[1];

    UVCSource src(videoSrc);
    ConvertColorFilter convert(CV_YUV2BGR_YUY2);
    DisplayFrameSink sink("Video");

    raft::map m;
    m += src >> convert >> sink;

    m.exe();
    return( EXIT_SUCCESS );
}
