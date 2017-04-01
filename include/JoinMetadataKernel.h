#pragma once

#include <raftinc/kernel.hpp>
#include "Metadata.h"

template <typename A, typename B>
class JoinMetadataKernelBase : public raft::kernel {
public:
    JoinMetadataKernelBase() {
        input.addPort<MetadataEnvelope<A>>("0");
        input.addPort<MetadataEnvelope<B>>("1");
    }

    virtual void OnMatch(MetadataEnvelope< std::pair<A, B> >& env) = 0;

    raft::kstatus run() override {
        auto &Av = input["0"].template peek<MetadataEnvelope<A>>();
        auto &Bv = input["1"].template peek<MetadataEnvelope<B>>();

        if(Av.Metadata().originId == Bv.Metadata().originId) {
            MetadataEnvelope< std::pair<A, B> > out;
            out.first = Av;
            out.second = Bv;

            OnMatch(out);

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

template <typename A, typename B>
class JoinMetadataKernel : public JoinMetadataKernelBase<A, B> {
public:
    typedef MetadataEnvelope< std::pair<A, B> > output_t;
    JoinMetadataKernel() {
        this->output.template addPort< output_t >("0");
    }

    void OnMatch(MetadataEnvelope<std::pair<A, B> > &env) override {
        this->output["0"].push(env);
    }
};


