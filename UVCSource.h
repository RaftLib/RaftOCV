#pragma once

#include <raft>

struct UVCSource_p;
class UVCSource : public raft::kernel {
    friend struct UVCSource_p;
    std::unique_ptr<UVCSource_p> p;
public:
    UVCSource();
    ~UVCSource();
    raft::kstatus run() override;
};
