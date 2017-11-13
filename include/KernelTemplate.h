#pragma once

#include <raft>

template <typename T> class PointerMember_;

template <typename T>
class KernelTemplate : public raft::kernel {
protected:
    std::unique_ptr< PointerMember_<T> > p;
public:
    KernelTemplate(PointerMember_<T>* ptr) : p(ptr){}
    virtual ~KernelTemplate() {};
};