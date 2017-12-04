#pragma once

#include <raft>

struct UVCSource_p;
class UVCSource : public raft::kernel {
    friend struct UVCSource_p;
    std::unique_ptr<UVCSource_p> p;

    std::string source;
public:
    UVCSource(const std::string& source = "/dev/video0");
    UVCSource(uint32_t& width, uint32_t& height, const std::string& source = "/dev/video0");
    ~UVCSource();
    raft::kstatus run() override;
};
