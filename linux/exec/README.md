## execve

```c
// https://elixir.bootlin.com/linux/v5.15/source/fs/exec.c#L2063
SYSCALL_DEFINE3(execve,
		const char __user *, filename,
		const char __user *const __user *, argv,
		const char __user *const __user *, envp)
{
	return do_execve(getname(filename), argv, envp);
}
```


## 参考
https://blog.csdn.net/faxiang1230/article/details/104096633
https://github.com/faxiang1230/Demo/blob/master/linux/elf/exec/do_execve.c

