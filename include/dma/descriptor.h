#pragma once
#include <cstdint>

namespace dma {

enum class CachingPolicy : uint8_t {
    WriteBack = 0,
    WriteThrough = 1,
};

struct DmaDescriptor {
    uint64_t src_addr = 0;
    uint64_t dst_addr = 0;
    uint64_t size = 0;           // bytes
    uint32_t stride = 0;         // 0 = contiguous (1D)
    uint32_t pitch = 0;          // bytes per row (for 2D)
    uint8_t  vc = 2;             // Virtual Channel (2 = bulk)
    CachingPolicy caching = CachingPolicy::WriteBack;
    uint32_t fence_id = 0;
    uint32_t event_id = 0;
    bool valid = false;
};

} // namespace dma