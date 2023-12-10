#include <iostream>

class Graphic {
public:
  virtual void draw() = 0;
  Graphic() = default;
  Graphic(const Graphic &) = default;
  Graphic(Graphic &&) = default;
  virtual ~Graphic() = default;
};

class Shape : public Graphic {
public:
  virtual void draw() override { std::cout << "Shape" << std::endl; }
};

class Dot : public Shape {
public:
  virtual void draw() override { std::cout << "Dot" << std::endl; }
};

class Circle : public Dot {
public:
  virtual void draw() override { std::cout << "Circle" << std::endl; }
};

class Rectangle : public Shape {
public:
  virtual void draw() override { std::cout << "Rectangle" << std::endl; }
};

class Exporter {
public:
  void Export(Shape *s) { s->draw(); }
  // void Export(Dot *s) { std::cout << "Exporter:Dot" << std::endl; }
};

class App {
public:
  void Export(Shape *s) {
    auto exporter = new Exporter();
    exporter->Export(s);
  }
};

// 继承所有基类里的函数调用运算符
template <typename... Ts> struct overload : Ts... {
  using Ts::operator()...;
};

// 基类的类型从传入的参数中推导
template <typename... Ts> overload(Ts...) -> overload<Ts...>;

#include <variant>
#include <any>
#include <vector>
#include <set>


int main() {
  // App app;
  // app.Export(new Dot());

  std::set<int> a;

  std::byte aa;
  std::cout << sizeof(aa) << std::endl;

  auto twice = overload{
      [](std::string &s) { s += s; },
      [](auto &i) { i *= 2; },
  };

  std::variant<int, std::string> var(42);
  std::visit(twice, var);

  std::vector<int> vi;

  std::any a = "hello";

  

  return 0;
}
