#pragma once

#include <cstdlib>
#include <chrono>
#include <utility>
#include "RaftOCV/calib3d/CalibrationTypes.h"
#include <memory>

struct Metadata_t {
    size_t originId = 0;
    std::chrono::time_point<std::chrono::system_clock> captureTime;
    Metadata_t();
};

template <typename T>
struct MetadataEnvelope : public T {
    typedef Metadata_t MetadataType;
    MetadataType metadata;

    MetadataEnvelope() = default;
    MetadataEnvelope(const MetadataEnvelope&) = default;

    MetadataEnvelope(const MetadataType& metadata) : metadata(metadata) {
    }

    template <typename... Args> MetadataEnvelope(const MetadataType& metadata,
                                                 Args&&... args) : T(std::forward<Args>(args)...),
                                                                   metadata(metadata) {
    }

    MetadataEnvelope& operator=(const T& t) {
        T::operator=(t);
        return *this;
    }

    const MetadataType& Metadata() const {
        return metadata;
    }
    MetadataType& Metadata() {
        return metadata;
    }
};

