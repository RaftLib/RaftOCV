#include <raft>
#include <DisplayFrameSink.h>
#include <ConvertColorFilter.h>

#include "UVCSource.h"

int main() {
    UVCSource src;
    ConvertColorFilter convert(CV_YUV2BGR_YUY2);
    DisplayFrameSink sink("Video");

    raft::map m;
    m += src >> convert >> sink;

    m.exe();
    return( EXIT_SUCCESS );
}
