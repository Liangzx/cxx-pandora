#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> ready(false);
std::atomic<int> data(0);

void writer_thread() {
    data.store(42, std::memory_order_relaxed);  // 写入数据
    ready.store(true, std::memory_order_release);  // 发布数据，写屏障
}

void reader_thread() {
    while (!ready.load(std::memory_order_acquire)) {  // 获取数据，读屏障
        // 等待 ready 变量变为 true
    }
    int local_data = data.load(std::memory_order_relaxed);  // 读取数据
    std::cout << "Data: " << local_data << std::endl;
}

int main() {
    std::thread writer(writer_thread);
    std::thread reader(reader_thread);

    writer.join();
    reader.join();

    return 0;
}
