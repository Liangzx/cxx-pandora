# CMD_SRC

## Linux | 如何查看Linux命令工具的源码？

需要了解Linux下的命令分为大致分为两类：内置命令与非内置命令。

### 内置命令

```shell
# 内置命令是Shell自带的， shell自己进行管理。help命令（help也是内置命令）来查看内置命令有哪些
help
# 查看系统当前使用的shell
env | grep SHELL

# bash源码路径: http://ftp.gnu.org/gnu/bash/
```

### 非内置命令源码

```shell
# 非内置命令，也即外部命令。
# 查看PATH中有哪些路径
echo $PATH

# 查看命令工具所在绝对路径(tee 为例)
which tee

# 搜索tee工具所属包
dpkg -S /usr/bin/tee
# coreutils: /usr/bin/tee tee工具所属源码包：coreutils

# 下载coreutils源码包
# 方法1：
sudo apt-get source coreutils
# sudo sed -i -- 's/#deb-src/deb-src/g' /etc/apt/sources.list && sudo sed -i -- 's/# deb-src/deb-src/g' /etc/apt/sources.list

# 方法二：
# http://www.gnu.org/software/
git clone git://git.sv.gnu.org/coreutils

```


[Linux | 如何查看Linux命令工具的源码](https://blog.csdn.net/zhengnianli/article/details/109476020)
