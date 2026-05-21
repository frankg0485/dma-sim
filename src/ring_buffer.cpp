#include "ring_buffer.h"
#include <cassert>

namespace dma {

RingBuffer::RingBuffer(uint32_t size) : buffer(std::vector<DmaDescriptor>(size, DmaDescriptor{})), mask(size - 1) {
    assert(size & (size - 1) == 0);
}

bool RingBuffer::push(const DmaDescriptor& desc) {
    if (is_full()) return false;

    buffer[tail] = desc;
    tail = (tail + 1) & mask;
    if (head == tail) full = true;
    return true;
}

bool RingBuffer::pop(DmaDescriptor& out) {
    if (is_empty()) return false;


    out = buffer[head];
    head = (head + 1) & mask;
    full = false;
    return true;
}

bool RingBuffer::is_empty() const {
    return head == tail && !full;
}

bool RingBuffer::is_full() const {
    return full;
}

uint32_t RingBuffer::capacity() const {
    return buffer.size();
}

}