cmake_minimum_required(VERSION 3.10)
project(executable)

set(CMAKE_CXX_STANDARD 17)

add_executable(executable src/main.cpp src/network_packages.cpp)

target_link_libraries( executable pthread pcap )