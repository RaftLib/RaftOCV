#pragma once


#include <KernelTemplate.h>

class DiffMask : public KernelTemplate<DiffMask> {
public:
    DiffMask();
    ~DiffMask() override;

    raft::kstatus run() override;
};


