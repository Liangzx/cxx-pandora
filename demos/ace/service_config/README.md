
[一个关于ACE Service Configurator的小例子](https://blog.csdn.net/rain_qingtian/article/details/24450275)

编译生成两个dll文件如下：
```shell
g++ -fPIC -shared -o libmydll.so my_dll.cpp -lACE
g++ -fPIC -shared -o libyamydll.so ya_my_dll.cpp -lACE
```

生成测试程序：
```shell
g++ -o test_dynamic_config test_dynamic_config.cpp -lACE
```

编写svc.conf, 先往svc.conf写入一下内容：

```text
dynamic MyDynamicObj Service_Object* libmydll.so:_make_MyDynamicObj ""

suspend MyDynamicObj

resume MyDynamicObj

remove MyDynamicObj
```

然后将svc.conf的内容改成：

```text
dynamic YaMyDynamicObj Service_Object* libyamydll.so:_make_YaMyDynamicObj ""

suspend YaMyDynamicObj

resume YaMyDynamicObj

remove YaMyDynamicObj
```

注意：
不能用相同的 obj 不然会段错误
