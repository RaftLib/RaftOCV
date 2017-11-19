#pragma once
#include <raft>
#include "JoinMetadataKernel.h"

class CombineFilter : public JoinMetadataKernelBase<cv::Mat, cv::Mat> {
public:
    CombineFilter();

    void OnMatch(MetadataEnvelope<std::pair<cv::Mat, cv::Mat>> &env) override;
};


