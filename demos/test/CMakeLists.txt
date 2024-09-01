cmake_minimum_required(VERSION 3.14)

project(sample1_test)

find_package(GTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(
  sample1_unittest
  sample1.cpp
  sample1_unittest.cpp
)

target_link_libraries(
  sample1_unittest
  # GTest::gtest
  # GTest::gtest_main
  /usr/local/lib/libgtest.a
  /usr/local/lib/libgtest_main.a
  pthread
)

include(GoogleTest)
gtest_discover_tests(sample1_unittest)
