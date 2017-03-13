#include <raft>
#include <opencv2/core/mat.hpp>

class UndistortFilter : public raft::kernel {
    cv::Mat map1, map2;

public:
    UndistortFilter(const cv::Mat &map1, const cv::Mat &map2);

    raft::kstatus run() override;
};


