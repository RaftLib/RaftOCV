#include <raft>
#include <opencv2/opencv.hpp>
#include <stdint.h>

struct StereoBMFilter_p;

class StereoBMFilter : public raft::kernel
{
    friend struct StereoBMFilter_p;

    std::unique_ptr<StereoBMFilter_p> p;

public:
    StereoBMFilter();
    ~StereoBMFilter();

    raft::kstatus run() override;
};


