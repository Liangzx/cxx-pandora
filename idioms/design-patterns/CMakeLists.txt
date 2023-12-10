cmake_minimum_required(VERSION 3.0.0)
project(design-patterns VERSION 0.1.0)

include(CTest)
enable_testing()


set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(creational-patterns)
add_subdirectory(hodp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
