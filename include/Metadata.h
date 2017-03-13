#pragma once

#include <cstdlib>
#include <chrono>
#include <utility>

struct Metadata_t {
    size_t originId = 0;
    std::chrono::time_point<std::chrono::system_clock> captureTime;
    Metadata_t();
};

template <typename T>
struct MetadataEnvelope : public T {
    Metadata_t metadata;

    MetadataEnvelope() = default;
    MetadataEnvelope(const MetadataEnvelope&) = default;

    MetadataEnvelope(const Metadata_t& metadata) : metadata(metadata) {
    }

    template <typename... Args> MetadataEnvelope(const Metadata_t& metadata,
                                                 Args&&... args) : T(std::forward<Args>(args)...),
                                                                   metadata(metadata) {
    }

    MetadataEnvelope& operator=(const T& t) {
        T::operator=(t);
        return *this;
    }

    const Metadata_t& Metadata() const {
        return metadata;
    }
    Metadata_t& Metadata() {
        return metadata;
    }
};