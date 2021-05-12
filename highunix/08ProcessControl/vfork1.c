#include "apue.h"

/*
 *
 * vfork()用法与fork()相似.但是也有区别,具体区别归结为以下3点:
 * 1. fork():子进程拷贝父进程的数据段，代码段. vfork():子进程与父进程共享数据段.
 * 2. fork():父子进程的执行次序不确定.
 * vfork():保证子进程先运行，在调用exec或_exit之前与父进程数据是共享的,在它调用exec
 * 或_exit之后父进程才可能被调度运行。
 * 3. vfork()保证子进程先运行，在她调用exec或_exit之后父进程才可能被调度运行。如果在
 * 调用这两个函数之前子进程依赖于父进程的进一步动作，则会导致死锁。
 * 4.当需要改变共享数据段中变量的值，则拷贝父进程。
 * */

int		globvar = 6;		/* external variable in initialized data */

int main( int argc, char ** argv)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;
	printf("before vfork\n");	/* we don't flush stdio */
	if ((pid = vfork()) < 0)
    {
		err_sys("vfork error");
	} 
    else if (pid == 0) 
    {		
		
	    printf("子进程\n");
        /* child */
		globvar++;				/* modify parent's variables */
		var++;
	    printf("子进程 终止\n");
		exit(0);
	 	//_exit(0);				/* child terminates */
	}
	else if (pid > 0)
    {
		while(1)
		{
			printf("父进程\n");
			printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
			sleep(2);
		}
	}

	/* parent continues here */
	//printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
	exit(0);
}
