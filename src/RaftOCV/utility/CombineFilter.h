#pragma once
#include <raft>
#include "JoinMetadataKernel.h"

class CombineFilter : public JoinMetadataKernelBase<cv::UMat, cv::UMat> {
public:
    CombineFilter();

    void OnMatch(MetadataEnvelope<std::pair<cv::UMat, cv::UMat>> &env) override;
};


