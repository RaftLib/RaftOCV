#pragma once
#include <raft>
#include "RaftOCV/utility/Metadata.h"

class SplitFilter : public raft::kernel {
public:
    typedef MetadataEnvelope<cv::UMat> output_t;
    SplitFilter();
    raft::kstatus run() override;
};


