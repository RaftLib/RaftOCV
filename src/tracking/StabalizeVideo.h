#pragma once

#include <raft>

struct StabalizeVideo_p;
class StabalizeVideo : public raft::kernel {
    friend struct StabalizeVideo_p;
    std::unique_ptr<StabalizeVideo_p> p;

public:
    raft::kstatus run() override;

    StabalizeVideo& operator []( const std::string &&portname );

    bool hasVideoOut = false;
    raft::kernel & videoOut();

    bool hasTxOut = false;
    raft::kernel & txOut();


    StabalizeVideo();
    ~StabalizeVideo();
};
