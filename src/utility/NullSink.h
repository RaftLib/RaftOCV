#pragma once

#include <raft>

template <typename T>
class NullSink : public raft::kernel {
public:
    NullSink() {
        input.addPort<T>("0");
    }

    raft::kstatus run() override {
        input["0"].recycle(input["0"].size());
        return raft::proceed;
    }
};