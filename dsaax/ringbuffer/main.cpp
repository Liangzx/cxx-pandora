#include <iostream>
#include <memory> // std::unique_ptr Defined in header <memory>
#include <thread> // std::thread
#include <mutex>  // std::mutex, std::lock_guard
using namespace std;

template <class T>
class circular_buffer
{
public:
    explicit circular_buffer(size_t size) : buf_(std::unique_ptr<T[]>(new T[size])),
                                            max_size_(size)
    {
        // empty constructor
    }

    void put(T item); // Adding Data
    T get();          // Retrieving Data
    void reset();
    bool empty() const;
    bool full() const;
    size_t capacity() const;
    size_t size() const;

private:
    std::mutex mutex_;
    std::unique_ptr<T[]> buf_;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t max_size_;
    bool full_ = 0;
};

template <class T>
bool circular_buffer<T>::empty() const
{
    // if head and tail are equal, we are empty
    return (!full_ && (head_ == tail_));
}

template <class T>
bool circular_buffer<T>::full() const
{
    // If tail is ahead the head by 1, we are full
    return full_;
}

template <class T>
void circular_buffer<T>::reset()
{
    std::lock_guard<std::mutex> lock(mutex_);
    head_ = tail_;
    full_ = false;
}

template <class T>
size_t circular_buffer<T>::capacity() const
{
    return max_size_;
}

template <class T>
size_t circular_buffer<T>::size() const
{
    size_t size = max_size_;

    if (!full_)
    {
        if (head_ >= tail_)
        {
            size = head_ - tail_;
        }
        else
        {
            size = max_size_ + head_ - tail_;
        }
    }

    return size;
}

template <class T>
void circular_buffer<T>::put(T item)
{
    std::lock_guard<std::mutex> lock(mutex_);

    buf_[head_] = item;

    if (full_)
    {
        tail_ = (tail_ + 1) % max_size_;
    }

    head_ = (head_ + 1) % max_size_;

    full_ = head_ == tail_;
}

template <class T>
T circular_buffer<T>::get()
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (empty())
    {
        return T();
    }

    // Read data and advance the tail (we now have a free space)
    auto val = buf_[tail_];
    full_ = false;
    tail_ = (tail_ + 1) % max_size_;

    return val;
}

int main()
{
    // Here’s an example using a buffer of 10 uint32_t entries :
    circular_buffer<uint32_t> circle(10);

    // Adding data is easy:
    uint32_t x = 100;
    circle.put(x);

    // And getting data is equally easy:
    x = circle.get();
    std::cout << x << endl;
}
