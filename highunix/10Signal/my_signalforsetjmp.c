#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
/**
  信号处理函数
    结果不对 仍然保持着SIGQUIT
 */
void sigdel(int signo) {
    printf("call sigdel\n");
}

int main(int argc, char *argv[])
{
    jmp_buf buf;
    sigset_t newmask, oldmask, pendmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    signal(SIGQUIT, sigdel);
    //阻塞SIGQUIT
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    if (setjmp(buf) != 0) 
    {
        sigemptyset(&newmask);
        sigpending(&pendmask);
        if (sigismember(&pendmask, SIGQUIT)) 
        {
            puts("block signal exist");
        } else {
            puts("block signal not exist");
        }
    }
    printf("sleep start\n");
    sleep(8);//这期间 ctrl+\ 发送SIGQUIT
    longjmp(buf, 1);
    return 0;
}
//由于setjmp 不会保留被阻塞的信号 所以输出
//"block signal not exist"