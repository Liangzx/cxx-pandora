// popen函数的实现
// https://www.cnblogs.com/li-hao/archive/2011/10/20/2219418.html

#include <errno.h>
#include <stdio.h>
#include <string.h>

void run_command(const char *cmd) {
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        // 这里可以记录错误，或者采取其他补救措施
        return;
    }

    // 读取命令的输出
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    // 关闭管道
    if (pclose(fp) == -1) {
        perror("Error closing pipe");
    }
}

int main() {
    char cmd[50] = "ls -l";
    run_command(cmd);
    return 0;
}
