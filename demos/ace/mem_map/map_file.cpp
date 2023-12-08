#include "ace/Mem_Map.h"
#include <cstring>
#include <iostream>

int main() {
  ACE_Mem_Map mem_map;
  auto shared_memory_file = "hello.txt";
  const size_t shared_memory_size = 1024; // 1KB
  const char *message = "This is the new content.";
  // handle
//   {
//     ACE_HANDLE shared_memory_handle =
//         ACE_OS::open(shared_memory_file, O_RDWR | O_CREAT | O_TRUNC);

//     std::cout << shared_memory_handle << std::endl;

//     if (shared_memory_handle != ACE_INVALID_HANDLE) {
//       off_t file_size = shared_memory_size;
//       ACE_OS::ftruncate(shared_memory_handle, file_size);
//       // 映射共享内存到进程地址空间
//       mem_map.map(shared_memory_handle, (size_t)-1, PROT_READ | PROT_WRITE,
//                   MAP_SHARED);

//       auto shared_memory_addr = mem_map.addr();
//       // 写入数据到共享内存
//       std::memcpy(shared_memory_addr, message, strlen(message));
//       // 解除映射
//       // mem_map.unmap(shared_memory_addr, shared_memory_size);
//       // // 关闭共享内存文件
//       // ACE_OS::close(shared_memory_handle);
//     }
//   }
  // memory
  {
    mem_map.map(shared_memory_file, -1, O_RDWR, ACE_DEFAULT_FILE_PERMS, PROT_READ | PROT_WRITE, MAP_SHARED);
    auto shared_memory_addr = mem_map.addr();
    std::memcpy(shared_memory_addr, message, strlen(message));
    // mem_map.unmap(shared_memory_addr, shared_memory_size);
  }

  return 0;
}
