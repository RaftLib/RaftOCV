#pragma once
#include <raft>

template <typename T>
class FromVector_ : public raft::kernel {
    std::vector<T> data;
    size_t idx = 0;
public:
    raft::kstatus run() override {
        if(idx >= data.size()){
            return raft::stop;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(33));

        output["0"].push(data[idx++]);
        return raft::proceed;
    }

    FromVector_(const std::vector<T> &data) : data(data) {
        output.addPort<T>("0");
    }
};