#pragma once
#include "dma/descriptor.h"
#include <vector>
#include <cstdint>

namespace dma {

class RingBuffer {
public:
    explicit RingBuffer(uint32_t size); // must be power of 2

    bool push(const DmaDescriptor& desc);
    bool pop(DmaDescriptor& out);
    bool is_empty() const;
    bool is_full() const;
    uint32_t capacity() const;

private:
    std::vector<DmaDescriptor> buffer;
    uint32_t head = 0;
    uint32_t tail = 0;
    uint32_t mask;
    bool full = false;
};

} // namespace dma