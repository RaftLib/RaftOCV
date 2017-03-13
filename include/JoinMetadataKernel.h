#pragma once

#include <raftinc/kernel.hpp>
#include "Metadata.h"

template <typename A, typename B>
class JoinMetadataKernel : public raft::kernel {
public:
    JoinMetadataKernel() {
        input.addPort<MetadataEnvelope<A>>("0");
        input.addPort<MetadataEnvelope<B>>("1");

        output.addPort<MetadataEnvelope< std::pair<A, B> >>("0");
    }
    raft::kstatus run() override {
        auto &Av = input["0"].template peek<MetadataEnvelope<A>>();
        auto &Bv = input["1"].template peek<MetadataEnvelope<B>>();

        if(Av.Metadata().originId == Bv.Metadata().originId) {
            auto &out = output["0"].template allocate<MetadataEnvelope< std::pair<A, B> > >(Av.Metadata());
            out.first = Av;
            out.second = Bv;

            output["0"].send();

            input["0"].unpeek();
            input["0"].recycle();
            input["1"].unpeek();
            input["1"].recycle();
        } else if(Av.Metadata().originId < Bv.Metadata().originId) {
            input["0"].unpeek();
            input["0"].recycle();
        } else {
            input["1"].unpeek();
            input["1"].recycle();
        }

        return raft::proceed;
    }
};


