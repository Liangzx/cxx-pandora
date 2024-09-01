#include <iostream>
#include <memory>

class Concept
{
public:
  virtual ~Concept() = default;
  virtual void Print() = 0;
  virtual Concept* Clone() = 0;

protected:
  Concept() = default;
};

template<typename T> // 在子类中多引入了一个或多个template parameter
class Model: public Concept
{
public:
  // 在子类的某一个构造函数中可以再引入一个template parameter
  // 此时应使用std::forward
  // 也可以在子类的某一个构造函数中直接使用主模板参数T
  template<typename U>
  explicit Model(U&& u) : mInstance(std::forward<U>(u))
  {
  }

  explicit Model(T&& t) : mInstance(std::move(t))
  {
  } // 此时应使用std::move
  void Print() override
  {
    mInstance.Print();
  }

private:
  T mInstance;
};

class Object
{
public:
  // forwarding constructor
  template<typename T>
  Object(T&& t) : mConcept(new Model<T>(std::forward<T>(t)))
  {
  }

  // destructor
  ~Object()
  {
    delete mConcept;
  }

  void Invoke()
  {
    mConcept->Print();
  }

private:
  Concept* mConcept; // 此处通过指针使用dynamic polymorphism
};
