
add_executable(leveldb_demo main.cpp)
include_directories(/usr/local/include/leveldb)
target_link_libraries(leveldb_demo PRIVATE -leveldb)