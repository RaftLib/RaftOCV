#pragma once

#include <stdint.h>
#include <raft>
#include <opencv2/core/mat.hpp>

class HeatmapFilter :
	public raft::kernel
{
	double minVal = 0, maxVal = 1000;

public:
	HeatmapFilter();
	~HeatmapFilter();

    raft::kstatus run() override;
};
