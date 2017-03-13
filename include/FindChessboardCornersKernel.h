#include <raft>
#include <opencv2/core/types.hpp>

class FindChessboardCornersKernel : public raft::kernel{
    cv::Size patternSize;
    bool enableOutput = true;
public:
    FindChessboardCornersKernel(const cv::Size &patternSize = { 5, 5});

    raft::kstatus run() override;
};


