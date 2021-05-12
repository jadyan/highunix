#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <sys/types.h> 
#include <unistd.h> 

void new_op(int, siginfo_t *, void *); 

int main(int argc,char**argv) 
{

    struct sigaction act; 
    int sig; 

    //sig=atoi(argv[1]); 
    sigemptyset(&act.sa_mask); 
    act.sa_flags=SA_SIGINFO;     //信号处理函数是带有三个参数的sa_sigaction
    act.sa_sigaction=new_op;     //新的信号安装机制，处理函数被调用的时候，不但可以得到信号编号，而且可以获悉被调用的原因以及产生问题的上下文的相关信息。

    //if(sigaction(sig,&act,NULL) < 0) 
    if(sigaction(SIGINT,&act,NULL) < 0)  /* ctrl-c*/
    { 
        perror("install sigal error"); 
        return -1 ;
    } 

    while(1) 
    { 
        sleep(2); 
        printf("wait for the signal\n"); 
    } 
    return 0 ;
} 

void new_op(int signum,siginfo_t *info,void *myact) 
{ 
    printf("receive signal %d\n", signum); 
    sleep(5); 
} 