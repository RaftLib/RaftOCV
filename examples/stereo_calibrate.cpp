#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <raft>
#include <ui/DisplayFrameSink.h>
#include <imgproc/ConvertColorFilter.h>
#include <stereo/StereoBMFilter.h>
#include <raftinc/print.tcc>
#include <Utility/DuplicateKernel.h>
#include <Utility/Metadata.h>
#include <Utility/JoinMetadataKernel.h>
#include <stereo/StereoCalibrateKernel.h>
#include <imgproc/UndistortFilter.h>
#include <imgproc/HeatmapFilter.h>
#include <stereo/StereoSubgraph.h>
#include "calib3d/FindChessboardCornersKernel.h"

#include "imgproc/ROIFilter.h"
#include "videoio/UVCSource.h"
#include "Utility/CombineFilter.h"
#include "../src/utility/PoolAllocator.h"

int main() {
    SetupOpenCVPoolAllocator();

    int offset = 48;
    using p_out = raft::print< std::vector<cv::Point2f>, '\n' >;

    uint32_t imgWidth = 640, imgHeight = 400;

    UVCSource src("/dev/video1");
    cv::Rect ps4eye(offset, 0, imgWidth * 2, imgHeight);
    cv::Rect imagesR(0, 0, imgWidth , imgHeight), imagesL(imgWidth, 0, imgWidth , imgHeight);

    DuplicateKernel< MetadataEnvelope<cv::Mat> > dup, dupR, dupL;

    ROIFilter roiImg(ps4eye);
    ConvertColorFilter convert(CV_YUV2BGR_YUY2);
    FindChessboardCornersKernel findChessboardL(cv::Size(6, 8)), findChessboardR(cv::Size(6, 8));
    DisplayFrameSink chessView("Chessboard"), calSink("undistorted"), stereoSink("Stereo");

    CombineFilter combine;
    JoinMetadataKernel<std::vector<cv::Point2f>,std::vector<cv::Point2f>> join;

    StereoCalibrateKernel sck(cv::Size(imgWidth, imgHeight), cv::Size(6, 8));
    sck.saveFile = "stereo_calib.yaml";

    DuplicateKernel<StereoCalibrateKernel::output_t> calibOutput;
    HeatmapFilter heatmap;

    StereoSubgraph stereoGraph;
    raft::map m;

    stereoGraph.connect(m);
    m += src >> roiImg >> convert >> stereoGraph.in();

    m += stereoGraph.newRightOutput() >> findChessboardR["0"]["debug"] >> combine["0"] >> chessView;
    m += findChessboardR["0"]["0"] >> join["0"];

    m += stereoGraph.newLeftOutput() >> findChessboardL["0"]["debug"] >> combine["1"];
    m += findChessboardL["0"]["0"] >> join["1"];

    m += join >> sck >> calibOutput;
    m += calibOutput["0"] >> stereoGraph.undistortR["calib"];
    m += calibOutput["1"] >> stereoGraph.undistortL["calib"];

    m += stereoGraph.newImageOutput() >> calSink;
    m += stereoGraph.newStereoOutput() >> heatmap >> stereoSink;
    m.exe();

    return( EXIT_SUCCESS );
}
