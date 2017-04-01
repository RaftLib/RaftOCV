#pragma once
#include <raft>

template <typename T>
class DropFilter : public raft::kernel{
public:
    DropFilter() {
        input.addPort<T>("0");
        output.addPort<T>("0");
    }
    raft::kstatus run() override {
        T& v = input["0"].template peek<T>();

        output["0"].recycle(output["0"].size());
        output["0"].push(v);

        input["0"].unpeek();
        input["0"].recycle();

        return raft::proceed;
    }
};


