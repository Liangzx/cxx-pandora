## 编译动态库

验证动态库中全局及静态变量

```shell
gcc -fPIC -c st.c
gcc -shared -o libst.so st.o
```
## 使用动态库
```shell
gcc -o set_val set_val.c -L. -lst
gcc -o get_val get_val.c -L. -lst
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.
```

## 参考
- [C++静态库与动态库](https://www.cnblogs.com/skynet/p/3372855.html)
- [linux 下动态链接实现原理](https://www.cnblogs.com/catch/p/3857964.html)
- [ELF文件解析](https://blog.csdn.net/lin___/article/details/104077452)
- [静态库与动态库全局变量及函数深入了解](https://blog.csdn.net/u012385733/article/details/80915360)
- [readelf 和 objdump 例子详解及区别 （ELF文件说明）](https://blog.csdn.net/lqy971966/article/details/106905237)
- [使用readelf和objdump解析目标文件](https://www.jianshu.com/p/863b279c941e)

```plant
@startuml
a -> b : To request
b --> a : To response
@enduml
```