#pragma once

#include <raft>

class BgRemoval : public raft::kernel {
    cv::Mat Mean;
    cv::Mat StdDev;
public:
    raft::kstatus run() override;

    BgRemoval(const cv::Mat &Mean, const cv::Mat &StdDev);
};

class BgModel : public raft::kernel{
public:
    BgModel();

    cv::Mat sum;
    cv::Mat sum2;
    //cv::Mat_<int> total;
    int total = 0;

    cv::Mat Mean() const;
    cv::Mat MeanF() const;
    cv::Mat StdDev() const;

    raft::kstatus run() override;
};


