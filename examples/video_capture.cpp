#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdint>
#include <iostream>
#include <raft>
#include <chrono>
#include <cinttypes>
#include <DisplayFrameSink.h>
#include <ConvertColorFilter.h>
#include <StereoBMFilter.h>
#include <HeatmapFilter.h>

#include "ROIFilter.h"
#include "UVCSource.h"

int main() {
    UVCSource src;
    ROIFilter roi(cv::Rect(0, 0, 100, 100));
    DisplayFrameSink sink("Video");
    ConvertColorFilter convert(CV_YUV2RGB_YUY2);

    raft::map m;
    m += src >> convert >> sink;
    /** start time for printing frame rate **/
    auto start =  std::chrono::high_resolution_clock::now();
    m.exe();
    return( EXIT_SUCCESS );
}
