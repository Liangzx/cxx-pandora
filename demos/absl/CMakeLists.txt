
add_subdirectory(status)
add_executable(absl_demo absl_demo.cpp)
include_directories(/usr/local/include/absl)
target_link_libraries(absl_demo PRIVATE -labsl_base)
target_link_libraries(absl_demo PRIVATE -labsl_strings)
target_link_libraries(absl_demo PRIVATE -labsl_raw_logging_internal)


