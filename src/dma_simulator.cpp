#include "dma/dma_simulator.h"
#include "ring_buffer.h"
#include <iostream>
#include <thread>
#include <format>
#include <unordered_map>

namespace dma {

struct DmaSimulator::Impl {
public:
    RingBuffer ring{256};           // power of 2

    std::thread engine_thread;
    std::mutex mtx;
    std::condition_variable cv;
    bool running = true;

    struct WaitFor {
        std::mutex mtx;
        std::condition_variable cv;
        bool finished = false;
    };
    std::unordered_map<uint32_t, std::unique_ptr<WaitFor>> event_wait_for;

    void engine_loop();

private:
    void process_descriptor(const DmaDescriptor& desc);
};

void DmaSimulator::Impl::process_descriptor(const DmaDescriptor& desc) {
    // TODO: process
    std::cout << "[Engine] Processing descriptor...\n";

    // update metadata for events
    if (!event_wait_for.contains(desc.event_id)) {
        event_wait_for.emplace(desc.event_id, std::make_unique<WaitFor>());
    }

    auto& wait_for = *event_wait_for[desc.event_id];
    {
        std::unique_lock<std::mutex> lk(wait_for.mtx);
        wait_for.finished = true;
    }

    std::cout << "[Engine] Completed descriptor (event_id=" << desc.event_id << ")\n";
    wait_for.cv.notify_all();
}

void DmaSimulator::Impl::engine_loop() {
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !ring.is_empty() || !running; });

        // Pop and process descriptors while available
        DmaDescriptor desc;
        while (ring.pop(desc)) {
            std::cout << std::format("[Engine] Fetched descriptor: src={:#x}, dst={:#x}, size={:#x}, vc={:d}\n", desc.src_addr, desc.dst_addr, desc.size, desc.vc);
            process_descriptor(desc);
        }
    }
}

DmaSimulator::DmaSimulator() : pimpl(std::make_unique<Impl>()) {
    pimpl->engine_thread = std::thread(&Impl::engine_loop, pimpl.get());
}

DmaSimulator::~DmaSimulator() {
    pimpl->running = false;
    pimpl->cv.notify_one();
    pimpl->engine_thread.join();
};

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
    pimpl->cv.notify_one();
    std::cout << "[Simulator] Doorbell rung\n";
}

void DmaSimulator::wait_for_event(uint32_t event_id) {
    if (!pimpl->event_wait_for.contains(event_id)) {
        pimpl->event_wait_for.emplace(event_id, std::make_unique<Impl::WaitFor>());
    }

    auto& wait_for = *pimpl->event_wait_for[event_id];
    {
        std::unique_lock<std::mutex> lk(wait_for.mtx);
        while (!wait_for.finished) {
            wait_for.cv.wait(lk, [&wait_for]{ return wait_for.finished; });
        }
    }
}

}