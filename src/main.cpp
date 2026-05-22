#include "dma/dma_simulator.h"
#include <iostream>

int main() {
    dma::DmaSimulator sim;
    std::cout << "Submitting descriptor...\n";
    auto id1 = sim.async_memcpy(0x1000, 0x2000, 4096);
    auto id2 = sim.async_memcpy(0x2000, 0x3000, 4096);

    std::cout << "Waiting for completion...\n";
    sim.wait_for_event(id1);
    sim.wait_for_event(id2);

    std::cout << "Done.\n";
    return 0;
}