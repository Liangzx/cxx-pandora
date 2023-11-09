```shell
g++ -shared -fPIC -o libstu.so student.cpp
g++ main.cpp libstu.so 
```

参考：
[C++二进制兼容](https://www.notion.so/Q-A-74e8d3a21ae748ce91b818e9abfe9cb8#916ddea12e544d378394f354d5eaaf43)