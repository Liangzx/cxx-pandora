add_executable(string_guide_demo string.cpp)
include_directories(/usr/local/include/absl)
target_link_libraries(string_guide_demo PRIVATE -labsl_base)
target_link_libraries(string_guide_demo PRIVATE -labsl_strings)
target_link_libraries(string_guide_demo PRIVATE -labsl_raw_logging_internal)


