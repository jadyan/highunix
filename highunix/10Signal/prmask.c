#include "apue.h"
#include <errno.h>


/*
	头文件：#include <signal.h>
	定义函数：int sigismember(const sigset_t *set, int signum);
	函数说明：sigismember()用来测试参数signum 代表的信号是否已加入至参数set 信号集里. 如果信号集里已有该信号则返回1, 否则返回0.
	
	include <signal.h>
	int sigprocmask(int how, const sigset_t *restrict set,sigset_t *restrict oset);
	//返回值：若成功，返回0；若出错，返回−1
		首先，若oset是非空指针，那么进程的当前信号屏蔽字通过oset返回。
		其次，若set是一个非空指针，则参数how指示如何修改当前信号屏蔽字。SIG_BLOCK是或操作，而SIG_SETMASK则是赋值操作。注意，不能阻塞SIGKILL和SIGSTOP信号。
*/
void pr_mask(const char *str)
{
	sigset_t	sigset;
	int			errno_save;

	errno_save = errno;		/* we can be called by signal handlers */
		
		
	if (sigismember(&sigset, SIGINT))
		printf(" SIGINT\n");
	if (sigismember(&sigset, SIGQUIT))
		printf(" SIGQUIT\n");
	if (sigismember(&sigset, SIGUSR1))
		printf(" SIGUSR1\n");
	if (sigismember(&sigset, SIGALRM))
		printf(" SIGALRM\n");
	
	
	if (sigprocmask(0, NULL, &sigset) < 0) 
	{
		err_ret("sigprocmask error");
	} else {
		printf("%s\n", str);
		if (sigismember(&sigset, SIGINT))
			printf(" SIGINT\n");
		if (sigismember(&sigset, SIGQUIT))
			printf(" SIGQUIT\n");
		if (sigismember(&sigset, SIGUSR1))
			printf(" SIGUSR1\n");
		if (sigismember(&sigset, SIGALRM))
			printf(" SIGALRM\n");

		/* remaining signals can go here  */

		printf("\n");
	}

	errno = errno_save;		/* restore errno */
}


int main(int argc, char **argv)
{
	pr_mask("add");
	sleep(10);
}