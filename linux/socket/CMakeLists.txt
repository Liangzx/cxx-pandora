cmake_minimum_required(VERSION 3.22)
project(socket)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tcp)
add_subdirectory(udp)
add_subdirectory(select)
add_subdirectory(poll)
add_subdirectory(epoll)
add_subdirectory(unix-domain-socket)
