#pragma once
#include "descriptor.h"
#include <memory>

namespace dma {

class DmaSimulator {
public:
    DmaSimulator();
    ~DmaSimulator();

    uint32_t async_memcpy(uint64_t src, uint64_t dst, uint64_t size);

    void submit_descriptor(const DmaDescriptor& desc);
    void ring_doorbell();

    void wait_for_event(uint32_t event_id);

private:
    // hide impl from users of this lib
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace dma