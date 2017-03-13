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
#include <raftinc/print.tcc>
#include <DuplicateKernel.h>
#include <Metadata.h>
#include <NullSink.h>
#include <JoinMetadataKernel.h>
#include <StereoCalibrateKernel.h>
#include "FindChessboardCornersKernel.h"

#include "ROIFilter.h"
#include "UVCSource.h"

int main() {
    int offset = 48;
    using p_out = raft::print< std::vector<cv::Point2f>, '\n' >;

    uint32_t imgWidth = 640, imgHeight = 400;

    cv::Rect imagesR(offset, 0, imgWidth , imgHeight), imagesL(offset + imgWidth, 0, imgWidth , imgHeight);

    UVCSource src;
    DuplicateKernel< MetadataEnvelope<cv::Mat> > dup(2);

    ROIFilter roiR(imagesR), roiL(imagesL);
    ConvertColorFilter convertR(CV_YUV2BGR_YUY2), convertL(CV_YUV2BGR_YUY2);
    FindChessboardCornersKernel findChessboardL(cv::Size(6, 8)), findChessboardR(cv::Size(6, 8));
    DisplayFrameSink sinkR("VideoR"), sinkL("VideoL");

    JoinMetadataKernel<std::vector<cv::Point2f>,std::vector<cv::Point2f>> join;
    StereoCalibrateKernel sck(cv::Size(imgWidth, imgHeight), cv::Size(6, 8));

    raft::map m;

    m += src >> dup;

    m += dup["0"] >> roiR >> convertR >> findChessboardR["0"]["debug"] >> sinkR;
    m += findChessboardR["0"]["0"] >> join["0"];

    m += dup["1"] >> roiL >> convertL >> findChessboardL["0"]["debug"] >> sinkL;
    m += findChessboardL["0"]["0"] >> join["1"];
    m += join >> sck;

/** start time for printing frame rate **/
    auto start =  std::chrono::high_resolution_clock::now();
    m.exe();
    return( EXIT_SUCCESS );
}
