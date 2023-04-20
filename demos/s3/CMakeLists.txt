find_package(aws-cpp-sdk-core REQUIRED)
find_package(aws-cpp-sdk-s3 REQUIRED)
find_package(aws-crt-cpp REQUIRED)

include_directories(/usr/local/include/aws)
add_executable(s3_demo main.cpp)

target_link_libraries(s3_demo -lcurl)
target_link_libraries(s3_demo aws-crt-cpp)
target_link_libraries(s3_demo aws-cpp-sdk-core)
target_link_libraries(s3_demo aws-cpp-sdk-s3)