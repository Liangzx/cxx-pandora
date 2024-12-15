# linkage

## refs

[storage_duration](https://en.cppreference.com/w/cpp/language/storage_duration)
[为什么标准库的模板变量都是inline的 ](https://www.cnblogs.com/apocelipes/p/16931410.html)


## note

A name can have external linkage, module linkage(since C++20), internal linkage, or no linkage.

### No linkage

Any of the following names declared at block scope have no linkage:
- variables that are not explicitly declared extern (regardless of the static modifier);
- local classes and their member functions;
- other names declared at block scope such as typedefs, enumerations, and enumerators.

```cpp
void function() {
    typedef int IntegerType; // No linkage, only visible in this block.

    enum Color {
        Red,
        Green,
        Blue
    }; // No linkage, only visible in this block.

    Color myColor = Red; // Enum and enumerator have no linkage.

    struct LocalStruct {
        int data;
    }; // LocalStruct has no linkage and is only visible in this block.

    LocalStruct ls;
    ls.data = 10;

    // Nested block
    {
        typedef float FloatType; // Still has no linkage, limited to this nested block.
    }
}
```

### Internal linkage

Any of the following names declared at namespace scope have internal linkage:
- variables, variable templates(since C++14), functions, or function templates declared static;
- non-template (since C++14) variables of non-volatile const-qualified type, unless
    - they are inline, (since C++17)
    - they are declared in the purview of a module interface unit (outside the private module fragment, if any) or module partition, (since C++20)
    - they are explicitly declared extern, or hey were previously declared and the prior declaration did not have internal linkage;
    - data members of anonymous unions.
- In addition, all names declared in unnamed namespaces or a namespace within an unnamed namespace, even ones explicitly declared extern, have internal linkage. (since C++11)

使用 static 关键字

```cpp
// 文件作用域中的 static 变量
static int internalVariable = 42;

// 文件作用域中的 static 函数
static void internalFunction() {
    // 函数体
}

// 文件作用域中的 static 模板函数 (C++14)
template<typename T>
static void internalTemplateFunction(T param) {
    // 函数体
}
```

非模板常量变量（自 C++14）

```cpp
// 具有内部链接的 const 变量
const int constInternalVar = 3; // 内部链接

// 显式声明为 extern 的 const 变量，具有外部链接
extern const int externalConstVar = 5; // 外部链接

// inline const 变量，即使它是 const，也具有外部链接 (C++17)
inline const int inlineConstVar = 7; // 外部链接

// 如果在一个模块接口单元中声明，则可能具有外部链接 (C++20)
export module myModule;
export const int moduleConstVar = 9; // 外部链接
```

匿名联合的数据成员

```cpp
union {
    int i;
    float f;
}; // i 和 f 都具有内部链接
```

匿名命名空间

```cpp
namespace {
    int anonymousNamespaceVar = 10; // 内部链接

    extern int stillInternalLinkageVar = 20; // 仍然是内部链接，即使使用了 extern
}
```

在模块中声明的名字（自 C++20）

```cpp
export module myModule;

// 这个变量在模块接口单元中声明，因此具有外部链接
export const int exportedConstVar = 42;

// 私有模块片段中的变量仍然具有内部链接
module :private;
const int privateModuleVar = 60; // 内部链接
```

### External linkage

Any of the following names declared at namespace scope have external linkage, unless they are declared in an unnamed namespace or their declarations are attached to a named module and are not exported(since C++20):
- variables and functions not listed above (that is, functions not declared static, non-const variables not declared static, and any variables declared extern);
- enumerations;
- names of classes, their member functions, static data members (const or not), nested classes and enumerations, and functions first introduced with friend declarations inside class bodies;
- names of all templates not listed above (that is, not function templates declared static).

Any of the following names first declared at block scope have external linkage:
- names of variables declared extern;
- names of functions.

命名空间作用域中的外部链接

```cpp
// 1. 非 static 的变量和函数
// 具有外部链接的全局变量
int globalVariable = 42;

// 具有外部链接的全局函数
void globalFunction() {
    // 函数体
}

// 枚举类型也具有外部链接
enum Color { Red, Green, Blue };

// 类型定义也具有外部链接
class MyClass {
public:
    static int staticDataMember; // 类的静态数据成员具有外部链接
    void memberFunction();       // 成员函数具有外部链接
};

// 在类体内声明为友元的函数也具有外部链接
class AnotherClass {
    friend void friendFunction();
};

// 模板也具有外部链接，除非它们是静态的
template<typename T>
void templateFunction(T param);

// 2. 使用 extern 声明的变量
// 使用 extern 声明的变量，即使它在文件作用域内声明，也具有外部链接
extern int externalVariable;
```

### 块作用域中的外部链接

```cpp
// 1. 使用 extern 声明的变量
void function() {
    // 使用 extern 声明的变量，具有外部链接
    extern int externalVariable;
}

// 2. 函数声明
void function() {
    // 函数声明总是具有外部链接
    void externalFunction();
}
```
