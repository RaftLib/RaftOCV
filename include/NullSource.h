#pragma once

#include <raft>

template <typename T>
class NullSource : public raft::kernel {
public:
    NullSource() {
        output.addPort<T>("0");
    }

    raft::kstatus run() override {
        return raft::proceed;
    }
};