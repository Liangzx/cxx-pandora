find_package(Boost COMPONENTS date_time filesystem system)
if (Boost_FOUND)
    message(STATUS "Boost_FOUND library found")
else ()
    message(FATAL_ERROR "Boost_NOT_FOUND library not found")
endif ()
# link boost
include_directories(${Boost_INCLUDE_DIRS}/boost)
message(STATUS "${Boost_INCLUDE_DIRS}")
add_executable(boost_fs fs.cpp)
target_link_libraries(boost_fs ${Boost_LIBRARIES})

