#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

static void sig_cld(int i)
{
    pid_t pid;
    int status;
    printf("SIGCLD received\n");
    // wait() 返回退出状态
    if((pid = wait(&status)) <0)
    {
        printf("wait error\n");
    }
    printf("pid = %d\n",pid);
}


int main()
{
    pid_t pid;
    if(signal(SIGCHLD,sig_cld) ==  SIG_ERR)
    {
        printf("signal error\n");
        exit(-1);
    }
    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    }
    else if(pid == 0)
    {
        //child
        printf("%d fork a new child %d\n",getppid(),getpid());
        sleep(1);
        exit(0);
    }
    else
    {
        int i,j;
        for(i=0;i<1000;i++)
        {
            for(j=0;j<10000000;j++)
            ;
            printf("this is [%d] %d\n", getpid(),i);
        }
    }
}
