#pragma once
#include <string>

namespace raft { class kernel; }
class Port;

class OptionalPort {
    std::string name;
    Port& port;

    bool active = false;
    bool isOutput = true;
public:

    raft::kernel& operator()();

    OptionalPort(const std::string &name, Port &port, bool isOutput = true);
};


