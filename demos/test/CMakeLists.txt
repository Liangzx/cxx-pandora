find_package(GTest)
enable_testing()

add_executable(
        sample1_unittest
        sample1_unittest.cpp
)

target_link_libraries(
        sample1_unittest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sample1_unittest)