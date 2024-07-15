# custom section

## refs

[Linux内核源码之自定义段](https://mp.weixin.qq.com/s/UzJtasEJ6gE8FJSpBxFawQ)
[example](https://github.com/javonhe/skill_of_c/blob/main/test_section.lds)

```shell
# 获取gcc自带链接脚本
ld --verbose > test_section.lds

# 修改并添加自定义section：mysec
_mysec_start = .;
.mysec           : { *(.mysec) }
_mysec_end = .;

# 编译运行
gcc -T test_section.lds -o elf_custom_section elf_custom_section.c

```

