#pragma once
#include <raft>
#include "utility/Metadata.h"

class SplitFilter : public raft::kernel {
public:
    typedef MetadataEnvelope<cv::Mat> output_t;
    SplitFilter();
    raft::kstatus run() override;
};


