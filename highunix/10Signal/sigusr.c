#include "apue.h"

/*
 * page 256
 *10.3 signal 函数
 * */
static void	sig_usr(int);	/* one handler for both signals */


/*
    pause函数使调用进程挂起直至捕捉到一个信号。
    #include <unistd.h>
    int pause(void);
    //返回值：−1，errno设置为EINTR
    只有执行了一个信号处理程序并从其返回时，pause才返回。在这种情况下，pause返回−1， errno设置为EINTR。
*/

int
main(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
	{
		err_sys("can't catch SIGUSR1");
	}
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
	{
		err_sys("can't catch SIGUSR2");
	}
	//不能注册SIGKILL SIGKILL	无法处理和忽略。中止某个进程
	//if (signal(SIGKILL, sig_usr) == SIG_ERR)
	//{
	//	err_sys("can't catch SIGKILL");
	//}
	for ( ; ; )
	{
		//pause 函数使自己休眠，当收到一个信号
		pause();	
	}
}

static void sig_usr(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1)
	{
		printf("received SIGUSR1\n");
	}
	else if (signo == SIGUSR2)
	{
		printf("received SIGUSR2\n");
	}
	//else if (signo == SIGKILL)
	//{
	//	printf("received SIGKILL\n");
	//	exit(0);
	//}
	else
	{
		err_dump("received signal %d\n", signo);
	}
}
