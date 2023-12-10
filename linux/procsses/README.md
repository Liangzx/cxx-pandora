## vfork fork clone

参考：
- [浅谈Linux vfork与fork简单对比分析](https://cloud.tencent.com/developer/article/1721538)
- [Linux中fork，vfork和clone详解](https://blog.csdn.net/gatieme/article/details/51417488)
- [Linux中waitpid()函数的用法](https://blog.csdn.net/Roland_Sun/article/details/32084825)
---
## exec族函数
参考：
- [linux进程---exec族函数](https://blog.csdn.net/u014530704/article/details/73848573)
---
## posix_spawn

```c
#include <spawn.h>
int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);
                
int posix_spawnp(pid_t *pid, const char *file,
                 const posix_spawn_file_actions_t *file_actions,
                 const posix_spawnattr_t *attrp,
                 char *const argv[], char *const envp[]);

```