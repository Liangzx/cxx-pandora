find_package(jsoncpp REQUIRED)
include_directories(/usr/local/include/openssl)
add_executable(json_demo main.cpp)
target_link_libraries(json_demo jsoncpp)