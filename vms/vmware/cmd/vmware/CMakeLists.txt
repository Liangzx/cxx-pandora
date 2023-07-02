project(vmware)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

add_executable(vmware main.cpp)

target_link_libraries(vmware pthread)

# find vddk lib
find_package(VDDK REQUIRED)
if (VDDK_FOUND)
    target_include_directories(vmware PRIVATE ${VDDK_INCLUDE_DIR})
    target_link_libraries(vmware ${VDDK_LIBRARY})
else ()
    message(FATAL_ERROR "VDDK library not found")
endif ()

