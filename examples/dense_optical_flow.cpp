#include <ui/DisplayFrameSink.h>
#include <videoio/VideoCaptureSource.h>
#include <QtWidgets/QApplication>
#include <utility/NullSink.h>
#include <utility/Metadata.h>
#include "tracking/DenseOpticalFlow.h"
#include "ui/UIMap.h"

cv::Size targetSize;

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    VideoCaptureSource src(argv[1]);
    DisplayFrameSink sink("Stabalize");
    UIMap m;

    DenseOpticalFlow flow;

    m += src >> flow >> sink;

    m.exec();

    return( EXIT_SUCCESS );
}