#pragma once
#include <raft>
#include <vector>

template <typename T>
class VectorizeData : public raft::kernel {
    std::vector<T> data;
public:
    const std::vector<T> Data() const { return data; };
    raft::kstatus run() override {
        T in;
        input["0"].pop(in);
        data.push_back(in);

        return raft::proceed;
    }
    VectorizeData() {
        input.addPort<T>("0");
    }
};