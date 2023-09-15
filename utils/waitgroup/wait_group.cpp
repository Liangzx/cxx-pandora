// https://github.com/google/marl/blob/main/include/marl/waitgroup.h
// https://blog.csdn.net/weixin_43869898/article/details/125925887

#include <atomic>
#include <condition_variable>
#include <mutex>

class WaitGroup {
public:
  WaitGroup(int initialCount = 0) : data_(std::make_shared<Data>()) {
    data_->count_ = initialCount;
  }
  // Add() increments the internal counter by count.
  void Add(int count = 1) { data_->count_ += count; }
  // Done() decrements the internal counter by count.
  void Done(int count = 1) {
    data_->count_ -= count;
    if (data_->count_ <= 0) {
      std::unique_lock<std::mutex> lck{data_->mux_};
      data_->cv_.notify_all();
    }
  }
  // Wait() blocks until the WaitGroup counter reaches zero or less.
  void Wait() {
    std::unique_lock<std::mutex> lck{data_->mux_};
    data_->cv_.wait(lck, [this]() { return data_->count_ <= 0; });
  }

private:
  struct Data {
    std::mutex mux_;
    std::atomic<int> count_ = {0};
    std::condition_variable cv_;
  };
  std::shared_ptr<Data> data_;
};