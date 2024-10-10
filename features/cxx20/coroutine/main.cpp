#include <coroutine>
#include <iostream>
#include <thread>

namespace Coroutine {
struct task
{
  struct promise_type
  {
    promise_type()
    {
      std::cout << "1.create promise object\n";
    }

    task get_return_object()
    {
      std::cout << "2.create coroutine return object, and the coroutine is "
                   "created now\n";
      return {std::coroutine_handle<>::from_promise(*this)};
    }

    std::suspend_never initial_suspend()
    {
      std::cout << "3.do you want to suspend the current coroutine?\n";
      std::cout << "4.don't suspend because return std::suspend_never, so "
                   "continue to execute coroutine body\n";
      return {};
    }

    std::suspend_never final_suspend() noexcept
    {
      std::cout << "13.coroutine body finished, do you want to suspend the "
                   "current coroutine?\n";
      std::cout << "14.don't suspend because return std::suspend_never, and "
                   "the continue will be automatically destroyed, bye\n";
      return {};
    }

    void return_void()
    {
      std::cout
        << "12.coroutine don't return value, so return_void is called\n";
    }

    void unhandled_exception()
    {
    }
  };

  std::coroutine_handle<> handle_;
};

struct awaiter
{
  bool await_ready()
  {
    std::cout << "6.do you want to suspend current coroutine?\n";
    std::cout << "7.yes, suspend because awaiter.await_ready() return false\n";
    return false;
  }

  void await_suspend(std::coroutine_handle<> handle)
  {
    std::cout << "8.execute awaiter.await_suspend()\n";
    std::thread([handle]() mutable {
      handle();
    }).detach();
    std::cout << "9.a new thread launched, and will return back to caller\n";
  }

  void await_resume()
  {
  }
};

task test()
{
  std::cout << "5.begin to execute coroutine body, the thread id="
            << std::this_thread::get_id() << "\n"; // #1
  co_await awaiter{};
  std::cout << "11.coroutine resumed, continue execute coroutine body now, "
               "the thread id="
            << std::this_thread::get_id() << "\n"; // #3
}

} // namespace Coroutine

int main()
{
  Coroutine::test();
  std::cout << "10.come back to caller because of co_await awaiter\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));

  return 0;
}
/**
 * 可以清晰的看到协程是如何创建的、co_await
 * 等待线程结束、线程结束后协程返回值以及协程销毁的整个过程。
 * 1. 输出内容中的 1、2、3 展示了协程创建过程，先创建 promise，再通过
 * promise.get_return_object() 返回 task，这时协程就创建完成。
 * 2. 协程创建完成之后是要立即执行协程函数呢？还是先挂起来？这个行为由
 * promise.initial_suspend() 来确定，由于它返回的是一个 std::suspend_never
 * 的awaiter，因此不会挂起协程，于是就立即执行协程函数。
 * 3. 执行协程到函数的 co_await awaiter 时，是否需要等待某个任务？返回 false
 * 表明希望等待，于是接着进入到 awaiter.await_suspend()，并挂起协程，在
 * await_suspend 中创建了一个线程去执行任务，之后就返回到 caller，caller
 * 这时候可以不用阻塞等待线程结束，可以做其它事情，需要注意的是 awaiter 同时也是一个 awaitable，因为它支持 co_await。
 * 4. 当线程开始运行的时候恢复挂起的协程，这时候代码执行会回到协程函数继续执行，这就是最终的目标：在一个新线程中去执行协程函数的打印语句。
 * promise.final_suspend 决定是否要自动销毁协程，返回 std::suspend_never 就自动销毁协程，否则需要用户手动去销毁。
 */
