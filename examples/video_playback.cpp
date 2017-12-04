#include <raft>
#include <RaftOCV.h>

int main(int argc, char** argv) {
    VideoCaptureSource src(argv[1]);
    DisplayFrameSink sink(argv[1]);

    raft::map m;
    m += src >> sink;

    m.exe();

    return( EXIT_SUCCESS );
}

