#pragma once
#include <raft>
#include <opencv2/core/mat.hpp>
#include <QtCore>
#include "RaftOCV/utility/Properties.h"

class DenseOpticalFlow_p;
class DenseOpticalFlow : public QObject, public raft::kernel{
    Q_OBJECT;

    DECLARE_PROPERTY(int, Skip) = 1;

    std::unique_ptr<DenseOpticalFlow_p> p;
    friend class DenseOpticalFlow_p;
public:
    raft::kstatus run() override;

    DenseOpticalFlow();
    ~DenseOpticalFlow();
};


