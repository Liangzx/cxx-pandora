project(citrix)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

add_executable(citrix main.cpp)

target_link_libraries(citrix pthread)
target_link_libraries(citrix xml2)
target_link_libraries(citrix curl)

# find xenserver lib
find_package(XENSERVER REQUIRED)
if (XENSERVER_FOUND)
    target_include_directories(citrix PRIVATE ${XENSERVER_INCLUDE_DIR})
    # link xenserver lib
    target_link_libraries(citrix ${XENSERVER_LIBRARY})
else ()
    message(FATAL_ERROR "xenserver library not found")
endif ()

## find and link xml lib -- cmake > 3.12
#find_package(Xml2 REQUIRED)
#if (LibXml2_FOUND)
#    include_directories(${LIBXML2_INCLUDE_DIR})
#    target_link_libraries(citrix ${LIBXML2_LIBRARY})
#else ()
#    message(FATAL_ERROR "xenserver library not found")
#endif ()
