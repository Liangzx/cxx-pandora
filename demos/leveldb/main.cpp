#include <cassert>
#include <iostream>
#include <string>

#include "leveldb/db.h"

int main() {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());
    std::string key = "hello";
    std::string value = "world";
    leveldb::Status s;
    s = db->Put(leveldb::WriteOptions(), key, value + " hello");
    s = db->Get(leveldb::ReadOptions(), key, &value);
    std::cout << value << std::endl;
//    if (s.ok()) s = db->Delete(leveldb::WriteOptions(), key1);

    delete db;

    return 0;
}