// enable_shared_from_this - overview, examples, and internals

#include <functional>
#include <memory>
#include <string>
#include <iostream>

// class Executor {
//  public:
//   // Executes a task asynchronously
//   void execute(const std::function<void(void)>& task);
//   //....
//  private:
//   /* Contains threads and a task queue */
// };

// class Processor {
//  public:
//   //...
//   // Processes data asynchronously. (implemented later)
//   void processData(const std::string& data);

//  private:
//   // Called in an Executor thread
//   void doProcessAndSave(const std::string& data) {
//     // 1. Process data
//     // 2. Save data to DB
//   }
//   //.. more fields including a DB handle..
//   Executor* executor;
// };

// // Processor receives some data from a Client class that holds a shared_ptr
// to
// // it:
// class Client {
//  public:
//   void someMethod() {
//     //...
//     processor->processData("Some Data");
//     //..do something else
//   }

//  private:
//   std::shared_ptr<Processor> processor;
// };

// void Processor::processData(const std::string& data) {
//   executor->execute([this, data]() {  //<--Bad Idea
//     // Executes in an Executor thread asynchronously
//     //'this' could be invalid here.
//     doProcessAndSave(data);
//   });
// }


class D : public std::enable_shared_from_this<D> {
 public:
};

int main() {
  auto ds = std::shared_ptr<D>(new D{});
  auto ds1 = ds;
  std::cout << ds.use_count() << std::endl;
  std::cout << ds.unique() << std::endl;

  return 0;
}
