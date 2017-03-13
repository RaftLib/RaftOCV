#include <raft>
#include <DisplayFrameSink.h>
#include <ConvertColorFilter.h>
#include <VideoCaptureSource.h>

int main() {
    VideoCaptureSource src("my-file.avi");
    DisplayFrameSink sink("Video");

    raft::map m;
    m += src >> sink;

    m.exe();
    return( EXIT_SUCCESS );
}

