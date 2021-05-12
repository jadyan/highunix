#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
/**
 * 信号处理函数
 */
void sigdel(int signo) {
    //do nothing
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
    if (sigsetjmp(buf, 1) != 0) 
    {
        sigpending(&pendmask);
        if (sigismember(&pendmask, SIGQUIT)) 
        {
            puts("block signal exist");
        }else{
            puts("block signal not exist");
        }
    }
    sleep(8);//这期间 ctrl+\ 发送SIGQUIT
    siglongjmp(buf, 1);
    return 0;
}
//由于sigsetjmp当第二个参数大于0的时候会保留被阻塞的信号，等于0功能和setjmp一样。所以以上代码输出
//"block signal exist