cmake_minimum_required(VERSION 3.22)
project(virt_samples)

set(CMAKE_CXX_STANDARD 17)
# find_package(virt REQUIRED)
add_executable(virt_samples main.cpp)
# libvirt
include_directories(/usr/include/libvirt)
target_link_libraries(virt_samples virt)
