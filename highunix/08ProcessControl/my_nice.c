#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
 
/*
 * 
 * 默认是0
 * nice值应该是熟悉Linux/UNIX的人很了解的概念了，我们都知它是反应一个进程“优先级”状态的值，其取值范围是-20至19，
 * 一共40个级别。这个值越小，表示进程”优先级”越高，而值越大“优先级”越低。我们可以通过nice命令来对一个将要执行的命令进行nice值设置，方法是：
 * [root@zorrozou-pc0 zorro]# nice -n 10 bash
 */ 

int main()
{
    time_t t1, t2, t3;
    time(&t1);
 
 
    int i = 0;
    int num = 10000;
 
 
    pid_t pid = fork();
 
 
    if (pid == 0) 
	{
		int ret= nice(29);
		printf("parent nice = %d by nice func \n", ret);
        int nice = getpriority(PRIO_PROCESS, 0);
        printf("parent nice = %d by getpriority fun\n", nice);
        for (i; i<num; --i)
            ;
        time(&t2);
        printf("parent pay %ld sec\n", t2-t1);
    } else if (pid > 0) 
	{
        setpriority(PRIO_PROCESS, 0, 19);
        int nice = getpriority(PRIO_PROCESS, 0);
        printf("child nice = %d\n", nice);
        for (i; i<num; --i)
            ;
        time(&t3);
        printf("child pay %ld sec\n", t3-t1);
    } else {
        perror("fork fail");
        return -1;
    }
 
    sleep(20);
    return 0;
}

