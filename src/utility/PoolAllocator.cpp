
#include <opencv2/core/mat.hpp>
#include "PoolAllocator.h"
#include <map>
#include <unordered_map>
#include <mutex>

class PoolAllocator : public cv::MatAllocator {
    mutable std::mutex m;
    mutable std::unordered_map<size_t, std::vector<cv::UMatData*> > freeblocks;
    mutable size_t misscount = 0, hitcount = 0, freeblocksCnt = 0;
public:
    cv::UMatData *allocate(int dims, const int *sizes, int type, void *data, size_t *step, int flags,
                           cv::UMatUsageFlags usageFlags) const override {
        if(data)
            return cv::Mat::getStdAllocator()->allocate(dims, sizes, type, data, step, flags, usageFlags);

        size_t total = CV_ELEM_SIZE(type);
        for( int i = dims-1; i >= 0; i-- )
        {
            if( step )
                step[i] = total;
            total *= sizes[i];
        }

        cv::UMatData * rtn = 0;
        {
            std::lock_guard<std::mutex> l(m);
            auto &freed = freeblocks[total];
            if (freed.size()) {
                freeblocksCnt--;
                rtn = freed.back();
                freed.pop_back();
            }
        }

        if (rtn) {
            hitcount++;
            return rtn;
        }

        return cv::Mat::getStdAllocator()->allocate(dims, sizes, type, data, step, flags, usageFlags);
    }

    bool allocate(cv::UMatData *data, int accessflags, cv::UMatUsageFlags usageFlags) const override {
        return cv::Mat::getStdAllocator()->allocate(data, accessflags, usageFlags);
    }

    void deallocate(cv::UMatData *u) const override {
        if( !(u->flags & cv::UMatData::USER_ALLOCATED) )
        {
            cv::Mat::getStdAllocator()->deallocate(u);
            return;
        }

        std::lock_guard<std::mutex> l(m);
        freeblocksCnt++;
        freeblocks[u->size].push_back(u);
    }
};

void SetupOpenCVPoolAllocator() {
    static PoolAllocator allocator;
    cv::Mat::setDefaultAllocator(&allocator);
}
