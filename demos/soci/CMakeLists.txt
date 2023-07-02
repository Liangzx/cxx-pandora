
add_executable(soci_demo main.cpp)
add_executable(soci_demo_test test.cpp)

include_directories(/usr/local/include/soci)
target_link_libraries(soci_demo PRIVATE -lsoci_core)
