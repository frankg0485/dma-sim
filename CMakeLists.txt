cmake_minimum_required(VERSION 3.20)
project(dma_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(dma_sim
    src/main.cpp
    src/dma_simulator.cpp
    src/ring_buffer.cpp
)

target_include_directories(dma_sim PRIVATE ${CMAKE_SOURCE_DIR}/include)
