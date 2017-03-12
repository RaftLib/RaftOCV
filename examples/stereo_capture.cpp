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
    int offset = 48;

    uint32_t imgWidth = 640;
    uint32_t imgHeight = 400;
    cv::Rect imagesR(offset, 0, imgWidth * 2, imgHeight);

    UVCSource src;
    ROIFilter roi(imagesR);
    ConvertColorFilter convert(CV_YUV2RGB_YUY2);
    StereoBMFilter stereo;
    HeatmapFilter heatmap;
    DisplayFrameSink sink("Video");

    raft::map m;
    m += src >> roi >> convert >> stereo >> heatmap >> sink;
    /** start time for printing frame rate **/
    auto start =  std::chrono::high_resolution_clock::now();
    m.exe();
    return( EXIT_SUCCESS );
}