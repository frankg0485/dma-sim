#include "dma/dma_simulator.h"
#include <iostream>

int main() {
    dma::DmaSimulator sim;

    std::cout << "Submitting descriptor...\n";
    sim.async_memcpy(0x1000, 0x2000, 4096);

    std::cout << "Waiting for completion...\n";
    sim.wait_for_event(0);

    std::cout << "Done.\n";
    return 0;
}