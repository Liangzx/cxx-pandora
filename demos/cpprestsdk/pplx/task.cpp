#include <pplx/pplx.h>

// g++ -std=c++11  -lboost_system -lcrypto -lssl -lcpprest

#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <pplx/pplxtasks.h>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

std::string now()
{
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d %H-%M-%S");
  auto str = oss.str();
  return str;
}

void sleep(int seconds)
{
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

pplx::task<std::string> create_print_task(const std::string& init_value)
{
  return pplx::create_task([init_value]() {
           std::cout << "Current value:" << init_value << std::endl;
           return std::string("Value 2");
         })
    .then([](std::string value) {
      std::cout << "Current value:" << value << std::endl;
      return std::string("Value 3");
    })
    .then([](std::string value) {
      std::cout << "Current value:" << value << std::endl;
      return std::string("Value 4");
    });
}

void test_task_chain()
{
  auto task_chain = create_print_task("Value 1");
  task_chain
    .then([](std::string value) {
      // uncomment this line to throw an exception.
      // throw std::runtime_error("An exception happened!");
      std::cout << "Result value: " << value << std::endl;
      return value;
    })
    .then([](pplx::task<std::string> previousTask) {
      // process exception
      try {
        previousTask.get();
      } catch (const std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
      }
    })
    .wait();
}

void test_group_tasks()
{
  auto sleep_print = [](int seconds, const std::string& info) {
    if (seconds > 0) {
      sleep(seconds);
    }

    std::cout << info << std::endl;
  };

  auto tasks = {pplx::create_task([sleep_print]() -> int {
                  sleep_print(2, "Task 1");
                  return 1;
                }),
                pplx::create_task([sleep_print]() -> int {
                  sleep_print(2, "Task 2");
                  return 2;
                }),
                pplx::create_task([sleep_print]() -> int {
                  sleep_print(4, "Task 3");
                  return 3;
                })};

  {
    std::cout << "=== when_all ===" << std::endl;

    auto joinTask = pplx::when_all(std::begin(tasks), std::end(tasks));
    auto result = joinTask.wait();
    std::cout << "All joined thread. result: " << result << std::endl;
  }

  {
    std::cout << "=== when_any ===" << std::endl;
    auto joinTask = pplx::when_any(std::begin(tasks), std::end(tasks))
                      .then([](std::pair<int, size_t> result) {
                        std::cout << "First task to finish returns "
                                  << result.first << " and has index "
                                  << result.second << std::endl;
                      });

    auto result = joinTask.wait();
    std::cout << "Any joined thread. result: " << result << std::endl;
  }
}

void test_cancellation()
{
  pplx::cancellation_token_source cts;
  std::cout << "Creating task..." << std::endl;
  auto task = pplx::create_task([cts] {
    bool moreToDo = true;
    while (moreToDo) {
      if (cts.get_token().is_canceled()) {
        return;
      } else {
        moreToDo = []() -> bool {
          std::cout << "Performing work at " << now() << std::endl;
          sleep(1);
          return true;
        }();
      }
    }
  });

  sleep(3);

  std::cout << "Canceling task... " << now() << std::endl;
  cts.cancel();

  std::cout << "Waiting for task to complete... " << now() << std::endl;
  task.wait();

  std::cout << "Done. " << now() << std::endl;
}

void test_cancellation_async()
{
  pplx::cancellation_token_source cts;
  auto task = pplx::task<void>([cts]() {
                std::cout << "Cancel continue_task" << std::endl;
                cts.cancel();
              })
                .then(
                  []() {
                    std::cout << "This will not run" << std::endl;
                  },
                  cts.get_token());

  try {
    if (task.wait() == pplx::task_status::canceled) {
      std::cout << "Taks has been canceled" << std::endl;
    } else {
      task.get();
    }
  } catch (const std::exception& e) {
    std::cout << "exception: " << e.what() << std::endl;
  }
}

int main(int argc, char* args[])
{
  std::cout << "==== pplx demo ====" << std::endl;
  test_task_chain();
  test_group_tasks();
  test_cancellation();
  test_cancellation_async();

  return 0;
}
