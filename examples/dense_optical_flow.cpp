#include <RaftOCV/ui/DisplayFrameSink.h>
#include <RaftOCV/videoio/VideoCaptureSource.h>
#include <QtWidgets/QApplication>
#include <RaftOCV/utility/NullSink.h>
#include <RaftOCV/utility/Metadata.h>
#include "RaftOCV/tracking/DenseOpticalFlow.h"
#include "RaftOCV/ui/UIMap.h"

cv::Size targetSize;

int main(int argc, char** argv) {
    VideoCaptureSource src(argv[1]);
    DisplayFrameSink sink("Stabalize");
    raft::map m;

    DenseOpticalFlow flow;

    m += src >> flow >> sink;

    m.exe();

    return( EXIT_SUCCESS );
}
