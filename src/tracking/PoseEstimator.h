#pragma once

#pragma once

#include <raft>

struct HumanPoseEstimationKernel_p;

class HumanPoseEstimationKernel : public raft::kernel{
    friend struct HumanPoseEstimationKernel_p;
    std::unique_ptr<HumanPoseEstimationKernel_p> p;
public:

    raft::kstatus run() override;

    HumanPoseEstimationKernel ();
    ~HumanPoseEstimationKernel ();
};
