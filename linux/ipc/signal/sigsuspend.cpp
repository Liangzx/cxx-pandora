#include <memory>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void handle(int s) {
    printf("信号干扰!\n");
}
#include <string>
#include <iostream>

void findSubStr() {
    std::string s = "abcabcbb";
    auto fast = 0;
    auto slow = 0;
    auto start = 0;
    auto len = 0;
    while (fast < s.length()) {
        auto c = s[fast];
        fast++;
        if (c == s[fast]) {
            if (len < fast - slow) {
                len = fast - slow;
                start = slow;
            }
            slow = fast;
        }
    }
    std::cout << s.substr(start, len) << std::endl;
}
int main() {
    findSubStr();
    //    int sum=0;
    //    int i;
    //    sigset_t sigs,sigt;
    //    sigemptyset(&sigs);
    //    sigemptyset(&sigt);
    //
    //    sigaddset(&sigs,SIGINT);
    //    //sigfillset(&sigs);
    //
    //    signal(SIGINT,handle);
    //
    //    sigprocmask(SIG_BLOCK,&sigs,0);
    //    for(i=0;i<10;i++)
    //    {
    //        sum+=i;
    //        sleep(5); // 模拟业务处理时间比较长
    //        sigsuspend(&sigt); // sigt 替换 sigs 即只有sigt 的信号不能唤醒进程
    //        sleep(5);
    //    }
    //    printf("%d\n",sum);
    //    sigprocmask(SIG_UNBLOCK,&sigs,0);
    //    printf("over!\n");
    //    std::shared_ptr<int> sh;
    //    std::unique_ptr<int> ss;
}
// https://blog.csdn.net/yellowston/article/details/89076362