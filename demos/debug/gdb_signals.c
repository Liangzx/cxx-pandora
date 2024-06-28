// Refs
// GDB调试之信号
// https://blog.csdn.net/bsp_mpu6050/article/details/107886322

// 在GDB中handle指令用于设置GDB对于信号的处理，可以输入help handle来查看
// nostop: 当被调试的程序收到信号时，GDB不会停住程序的运行，但会打出消息告诉你收到这种信号
// stop: 当被调试的程序收到信号时，GDB会停住你的程序
// print: 当被调试的程序收到信号时，GDB会显示出一条信息
// noprint: 当被调试的程序收到信号时，GDB不会告诉你收到信号的信息
// pass 、noignore: 当被调试的程序收到信号时，GDB不处理信号。这表示，GDB会把这个信号交给被调试程序会处理
// nopass、ignore: 当被调试的程序收到信号时，GDB不会让被调试程序来处理这个信号

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signalHandle(int para)
{
    printf("in signal handle,signal num:%d\n", para);
}

void signalHandle2(int para)
{
    printf("in signal handle,signal num:%d\n", para);
    exit(0);
}


int main(void)
{
    signal(SIGUSR1, signalHandle);
    signal(SIGUSR2, signalHandle2);

    while(1)
    {
        printf("press any key to send a signal\n");
        getchar();
        raise(SIGUSR1);
     }

    return 0;
}
