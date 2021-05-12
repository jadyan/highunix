#include "apue.h"

/*
 * 进程会计
 第8章，8.14 进程会计 page 269
 
 会计记录对应于进程而不是程序。在fork之后，内核为子进程初始化一个记录，而不是在一个新程序被执行时。
 虽然exec并不创建一个新的会计记录，但相应记录中的命令名改变了，AFORK标志则被清除。这意味着
 ，如果一个进程顺序执行了三个程序(A exec B,B exec C,最后C exit)，但只写一个会计记录。在该记录中的命令名对应于程序C，但CPU时间是程序A、B、C之和。
 * */
int main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if (pid != 0)
	{
		/* 父进程 */
		sleep(2);
		exit(2);				/* terminate with exit status 2 */
	}

	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if (pid != 0) 
	{		
		/* 子进程 fork出来第一个子进程 后结束*/
		sleep(4);
		abort();				/* terminate with core dump */
	}

	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if (pid != 0) 
	{	
		/* 上一个子进程 fork 出来的*/	
		/* second child */
		execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
		exit(7);				/* shouldn't get here */
	}

	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if (pid != 0) 
	{
		/* third child */
		sleep(8);
		exit(0);				/* normal exit */
	}

	sleep(6);					/* fourth child */
	kill(getpid(), SIGKILL);	/* terminate w/signal, no core dump */
	exit(6);					/* shouldn't get here */
}
