#include "Metadata.h"

Metadata_t::Metadata_t() {
    captureTime = std::chrono::system_clock::now();
}