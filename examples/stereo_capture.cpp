#include <opencv2/highgui/highgui.hpp>
#include <raft>
#include <ui/DisplayFrameSink.h>
#include <imgproc/ConvertColorFilter.h>
#include <imgproc/HeatmapFilter.h>

#include "imgproc/ROIFilter.h"
#include "videoio/UVCSource.h"
#include "pcl/PointCloudFilter.h"
#include <imgproc/UndistortFilter.h>
#include <stereo/StereoCalibrateKernel.h>
#include <stereo/StereoSubgraph.h>
#include <pcl/PointCloudViewer.h>
#include "../src/utility/PoolAllocator.h"

int main(int argc, const char** argv) {
    SetupOpenCVPoolAllocator();

    const char* calibFile = "stereo_calib.2.yaml";
    if(argc > 1)
        calibFile = argv[1];



    int offset = 48;
    uint32_t imgWidth = 640, imgHeight = 400;
    cv::Rect imagesR(offset, 0, imgWidth * 2, imgHeight);

    UVCSource src("/dev/video1");
    ROIFilter roi(imagesR);
    ConvertColorFilter convert(CV_YUV2BGR_YUY2);
    HeatmapFilter heatmap;
    DisplayFrameSink sink("Video"), display("display");

    StereoCalibrationResults results(calibFile);
    StereoSubgraph stereoGraph(results);

    PointCloudViewer viewer;

    raft::map m;

    stereoGraph.connect(m);
    m += src >> roi >> convert >> stereoGraph.in();
    m += stereoGraph.newStereoOutput() >> heatmap >> sink;
    m += stereoGraph.newImageOutput() >> display;
    m += stereoGraph.pointCloudOutput() >> viewer;

    m.exe();
    return( EXIT_SUCCESS );
}