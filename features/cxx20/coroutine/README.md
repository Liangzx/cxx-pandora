# coroutine

## 无栈协程成为 C++20 协程标准

```text
协程分类：协程分为无栈协程和有栈协程两种，无栈指可挂起/恢复的函数，有栈协程则相当于用户态线程。
切换成本：有栈协程切换的成本是用户态线程切换的成本，而无栈协程切换的成本则相当于函数调用的成本。
无栈协程和线程的区别：无栈协程只能被线程调用，本身并不抢占内核调度，而线程则可抢占内核调度。
协程函数与普通函数的区别：普通函数执行完返回，则结束，协程函数可以运行到一半，返回并保留上下文；下次唤醒时恢复上下文，可以接着执行。
协程与多线程：
    协程适合 IO 密集型程序，一个线程可以调度执行成千上万的协程，IO 事件不会阻塞线程。
    多线程适合 CPU 密集型场景，每个线程都负责 cpu 计算，cpu 得到充分利用。
协程与异步：
    都是不阻塞线程的编程方式，但是协程是用同步的方式编程、实现异步的目的，比较适合代码编写、阅读和理解。
    异步编程通常使用 callback 函数实现，将一个功能拆分到不同的函数，相比协程编写和理解的成本更高。
```

## C++20 为什么选择无栈协程？

```text
1. 有栈（stackful）协程通常的实现手段是在堆上提前分配一块较大的内存空间（比如 64K），
也就是协程所谓的“栈”，参数、return address 等都可以存放在这个“栈”空间上。如果需要协程切换，
那么通过 swapcontext 一类的形式来让系统认为这个堆上空间就是普通的栈，这就实现了上下文的切换。
2. 有栈协程最大的优势就是侵入性小，使用起来非常简便，已有的业务代码几乎不需要做什么修改，但是 C++20 最终还是选择了使用无栈协程，主要出于下面这几个方面的考虑。
- 栈空间的限制：有栈协程的“栈”空间普遍是比较小的，在使用中有栈溢出的风险；而如果让“栈”空间变得很大，对内存空间又是很大的浪费。
    无栈协程则没有这些限制，既没有溢出的风险，也无需担心内存利用率的问题。
- 性能：有栈协程在切换时确实比系统线程要轻量，但是和无栈协程相比仍然是偏重的，
    这一点虽然在目前的实际使用中影响没有那么大（异步系统的使用通常伴随了 IO，相比于切换开销多了几个数量级），
    但也决定了无栈协程可以用在一些更有意思的场景上
关于协程的储存空间：
    - C++ 的设计是无栈协程, 所有的局部状态都储存在堆上;
    - 储存协程的状态需要分配空间，分配 frame 的时候会先搜索 promise_type 有没有提供 operator new, 其次是搜索全局范围；
    - 有分配就可能会有失败，如果写了 get_return_object_on_allocation_failure() 函数，那就是失败后的办法，代替 get_return_object() 来完成工作（需要 noexcept）；
    - 协程结束以后的释放空间也会先在 promise_type 里面搜索 operator delete，其次搜索全局范围；
    - 协程的储存空间只有在运行完 final_suspend 之后才会析构，或者得显式调用 coro.destroy()，否则协程的存储空间就永远不会释放，
      如果在 final_suspend 那里停下，那么就得在包装函数里面手动调用 coro.destroy()，不然就会漏内存；
    - 如果已经运行完毕 final_suspend，或者已经被 coro.destroy() 给析构了，那么协程的储存空间已经被释放，再次对 coro 做任何的操作都会导致 seg fault。
```

## 无栈协程是普通函数的泛化

```text
- 无栈协程是一个可以暂停和恢复的函数，是函数调用的泛化，这是为什么呢？我们知道一个函数的函数体（function body）
  是顺序执行的，执行完之后将结果返回给调用者，没办法挂起它并稍后恢复它，只能等待它结束。
- 而无栈协程则允许我们把函数挂起，然后在任意需要的时刻去恢复并执行函数体，相比普通函数，
  协程的函数体可以挂起并在任意时刻恢复执行，因此，从这个角度来说，无栈协程是普通函数的泛化。
```

## C++20 协程的“微言大义”

```text
- C++20 提供了三个新关键字（co_await、co_yield 和 co_return），如果一个函数中存在这三个关键字之一，那么它就是一个协程。
- 编译器会为协程生成许多代码以实现协程语义，会生成什么样的代码？怎么实现协程的语义？协程的创建是怎样的？co_await 机制是怎样的？

协程帧（coroutine frame）
- 当 caller 调用一个协程的时候会先创建一个协程帧，协程帧会构建 promise 对象，再通过 promise 对象产生 return object。
- 协程帧中主要有这些内容：
  - 协程参数；
  - 局部变量；
  - promise 对象。
- 这些内容在协程恢复运行的时候需要用到，caller 通过协程帧的句柄 std::coroutine_handle 来访问协程帧。

promise_type
- promise_type 是 promise 对象的类型，promise_type 用于定义一类协程的行为，包括协程创建方式、
  协程初始化完成和结束时的行为、发生异常时的行为、如何生成 awaiter 的行为以及 co_return 的行为等。
- promise 对象可以用于记录/存储一个协程实例的状态，每个协程桢与每个 promise 对象以及每个协程实例是一一对应的。

coroutine return object
- 它是 promise.get_return_object() 方法创建的，一种常见的实现手法会将 coroutine_handle 存储到
  coroutine object 内，使得该 return object 获得访问协程的能力。

std::coroutine_handle
- 协程帧的句柄，主要用于访问底层的协程帧、恢复协程和释放协程帧。
- 可通过调用 std::coroutine_handle::resume() 唤醒协程。

co_await、awaiter、awaitable
- co_await：一元操作符；
- awaitable：支持 co_await 操作符的类型；
- awaiter：定义了 await_ready、await_suspend 和 await_resume 方法的类型。
- co_await expr 通常用于表示等待一个任务（可能是 lazy 的，也可能不是）完成。co_await expr 时，expr 的类型需要是一个 awaitable，
  而该 co_await表达式的具体语义取决于根据该 awaitable 生成的 awaiter。
- 看起来和协程相关的对象还不少，这正是协程复杂又灵活的地方，可以借助这些对象来实现对协程的完全控制，实现任何想法。
  但是，需要先要了解这些对象是如何协作的，掌握了协程的原理，写协程应用也会游刃有余。

```

## Execution

```text
Each coroutine is associated with
- the promise object, manipulated from inside the coroutine.
  The coroutine submits its result or exception through this object.
  Promise objects are in no way related to std::promise.

- the coroutine handle, manipulated from outside the coroutine.
  This is a non-owning handle used to resume execution of the coroutine or to destroy the coroutine frame.

- the coroutine state, which is internal, dynamically-allocated storage (unless the allocation is optimized out), object that contains
  - the promise object
  - the parameters (all copied by value)
  - some representation of the current suspension point, so that a resume knows where to continue, and
    a destroy knows what local variables were in scope
  - local variables and temporaries whose lifetime spans the current suspension point.

When a coroutine begins execution, it performs the following:
- allocates the coroutine state object using operator new.
- copies all function parameters to the coroutine state: by-value parameters are moved or copied,
  by-reference parameters remain references (thus, may become dangling, if the coroutine is resumed
  after the lifetime of referred object ends — see below for examples).
- calls the constructor for the promise object. If the promise type has a constructor that
  takes all coroutine parameters, that constructor is called, with post-copy coroutine arguments.
  Otherwise the default constructor is called.
- calls promise.get_return_object() and keeps the result in a local variable.
  The result of that call will be returned to the caller when the coroutine first suspends.
  Any exceptions thrown up to and including this step propagate back to the caller, not placed in the promise.
- calls promise.initial_suspend() and co_awaits its result. Typical Promise types either return a
  std::suspend_always, for lazily-started coroutines, or std::suspend_never, for eagerly-started coroutines.
- when co_await promise.initial_suspend() resumes, starts executing the body of the coroutine.
```

## Promise

## refs

[C++之深入解析C++20协程的原理和应用](https://blog.csdn.net/Forever_wj/article/details/130237117)
[Coroutines](https://en.cppreference.com/w/cpp/language/coroutines)
[【NO.40】C++开发中使用协程需要注意的问题](https://github.com/0voice/cpp_backend_awsome_blog/blob/main/%E3%80%90NO.40%E3%80%91C%2B%2B%E5%BC%80%E5%8F%91%E4%B8%AD%E4%BD%BF%E7%94%A8%E5%8D%8F%E7%A8%8B%E9%9C%80%E8%A6%81%E6%B3%A8%E6%84%8F%E7%9A%84%E9%97%AE%E9%A2%98.md)
[【并发编程二十一】c++20协程(co_yield、co_return、co_await ）](https://www.cnblogs.com/lidabo/p/17223367.html)
[co_await的基本语义](http://purecpp.cn/detail?id=2278)
[c++20的协程学习记录（一）： 初探co_await和std::coroutine_handle<>](https://cloud.tencent.com/developer/article/2375979)
[Coroutine Theory](https://lewissbaker.github.io/2017/09/25/coroutine-theory)
[CppCoro](https://github.com/lewissbaker/cppcoro)
[理解C++20 Coroutine: co_await与Awaiter](https://uint128.com/2022/02/21/%E7%90%86%E8%A7%A3C-20-Coroutine-co-await%E4%B8%8EAwaiter/)
[Cpp中还不完美的Coroutines](https://joytsing.cn/posts/55948/)
[探究C++20协程(1)——C++协程概览](https://blog.csdn.net/weixin_45605341/article/details/137693546)
