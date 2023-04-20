find_path(VDDK_INCLUDE_DIR vixDiskLib.h ${CMAKE_SOURCE_DIR}/3rdparty/vddk/v6.0.0/include)
find_library(VDDK_LIBRARY NAMES vixDiskLib PATHS ${CMAKE_SOURCE_DIR}/3rdparty/vddk/v6.0.0/lib64)

if (VDDK_INCLUDE_DIR AND VDDK_LIBRARY)
    set(VDDK_FOUND TRUE)
endif ()