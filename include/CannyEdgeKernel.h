#pragma once

#include <raft>

class CannyEdgeKernel : public raft::kernel{
 public:
    double threshold1;
    double threshold2;
    int apertureSize = 3;
    bool L2gradient = false;

    raft::kstatus run() override;

    CannyEdgeKernel(double threshold1 = 100, double threshold2 = 100, int apertureSize = 3, bool L2gradient = false);
};

