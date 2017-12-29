#pragma once

#include <raft>

class BgRemoval : public raft::kernel {
    cv::UMat Mean;
    cv::UMat StdDev;
public:
    raft::kstatus run() override;

    BgRemoval(const cv::UMat &Mean, const cv::UMat &StdDev);
};

class BgModel : public raft::kernel{
public:
    BgModel();

    cv::UMat sum;
    cv::UMat sum2;
    //cv::UMat_<int> total;
    int total = 0;

    cv::UMat Mean() const;
    cv::UMat MeanF() const;
    cv::UMat StdDev() const;

    raft::kstatus run() override;
};


