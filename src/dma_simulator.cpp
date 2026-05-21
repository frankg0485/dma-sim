#include "dma/dma_simulator.h"
#include "ring_buffer.h"
#include <iostream>
#include <thread>

namespace dma {

struct DmaSimulator::Impl {
    RingBuffer ring{256};           // power of 2
    std::atomic<bool> doorbell{false};
};

DmaSimulator::DmaSimulator() : pimpl(std::make_unique<Impl>()) {}
DmaSimulator::~DmaSimulator() = default;

void DmaSimulator::async_memcpy(uint64_t src, uint64_t dst, uint64_t size) {
    auto desc = DmaDescriptor{};
    desc.src_addr = src;
    desc.dst_addr = dst;
    desc.size = size;
    desc.valid = true;

    submit_descriptor(desc);
    ring_doorbell();
}

void DmaSimulator::submit_descriptor(const DmaDescriptor& desc) {
    if (!pimpl->ring.push(desc)) {
        std::cerr << "[Simulator] Ring full, dropping descriptor\n";
    }
}

void DmaSimulator::ring_doorbell() {
    pimpl->doorbell.store(true);

    std::cout << "[Simulator] Doorbell rung\n";
}

void DmaSimulator::wait_for_event(uint32_t event_id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}