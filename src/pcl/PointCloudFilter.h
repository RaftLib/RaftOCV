#pragma once
#include <raft>

class PointCloudFilter : public raft::kernel {
public:
    PointCloudFilter();

    raft::kstatus run() override;
};


