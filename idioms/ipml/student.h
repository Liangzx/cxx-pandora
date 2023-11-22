class People {
public:
  // Note: 更改func1 func2 顺序会出现 main 调用不同函数及二进制不兼容
  virtual void func1() = 0;
  virtual void func2() = 0;
};

class Student : public People {
public:
    Student();
  void printa();
  void func2() override;
  void func1() override;

private:
  int c_ = 3;
  int b_ = 2;
  int a_ = 1;
};