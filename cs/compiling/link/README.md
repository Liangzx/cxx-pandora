## 实验

### 实验环境

```text
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 18.04.6 LTS
Release:        18.04
Codename:       bionic

gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
```

### 生成动态库及可执行文件

```shell
gcc -fpic -shared -g -o libadd_debug.so add.c
gcc -fpic -shared -g -o libsub_debug.so sub.c
gcc -o main main.c ./libadd_debug.so ./libsub_debug.so -g
```
### 分析调式

#### 查看 main program header

```shell
readelf -l main
```
从上面可以看出，`.plt`、`.plt.got`和`.text`等位于同一个 segment（代码段），`.got、.got.plt`和`.data`等位于同一个 segment（数据段）

#### 分析 .plt section 的作用

```shell
objdump -d --section=.plt main
```



### 参考：

https://www.binss.me/blog/plt-and-got/

https://csstormq.github.io/blog/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%B3%BB%E7%BB%9F%E7%AF%87%E4%B9%8B%E9%93%BE%E6%8E%A5%EF%BC%8814%EF%BC%89%EF%BC%9A.plt%E3%80%81.plt.got%E3%80%81.got%20%E5%92%8C%20.got.plt%20section%20%E4%B9%8B%E9%97%B4%E7%9A%84%E5%8C%BA%E5%88%AB