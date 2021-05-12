#include	"apue.h"
#include	<sys/wait.h>

/*
 *SIGCLD 语义
	page265
//    10.7
//    SIGCHLD的语义为:子进程状态改变后产生此信号，父进程需要调用一个wait函数以确定发生了什么。
//    对于SIGCLD的早期处理方式如下:
//	1、如果进程特地设置该信号的配置为SIG_IGN,则调用进程的子进程将不产生僵死进程。
	2、如果将SIGCLD的配置设置为捕捉，则内核立即检查是否有子进程准备好被等待，如果是这样，则调用SIGCLD处理程序。
	一般的,父进程在生成子进程之后会有两种情况，一种是父进程继续去做别的事情,另一种是父进程啥都不做,一直在wait子进程退出.
	
	SIGCHLD信号就是为这第一种情况准备的,它让父进程去做别的事情,而只要父进程注册了处理该信号的函数,在子进程退出时就会调用该函数,
	在该函数中又可以调用wait得到终止的子进程的状态。处理信号的函数执行完后，再继续做父进程的事情.
	
	如果父进程在fork之后调用wait，就会阻塞，直到有一个子进程退出。
	如果父进程在fork之前先signal(SIGCLD, sig_cld)，即注册了SIGCLD的信号处理函数。
	然后做自己的事情。当子进程退出时，会给父进程发送一个SIGCLD信号。然后sig_cld函数就会执行。
	可以在sig_cld函数中调用wait获得子进程退出时的状态，并且此时wait不会阻塞。 当sig_cld函数执行完后，父进程又继续做自己的事情。
//https://blog.csdn.net/drdairen/article/details/52350456
//    */
static void	sig_cld(int);

int main()
{
	pid_t	pid;

/*
	如果父进程在fork之前先signal(SIGCLD, sig_cld)，即注册了SIGCLD的信号处理函数。
	然后做自己的事情。当子进程退出时，会给父进程发送一个SIGCLD信号。然后sig_cld函数就会执行。
	可以在sig_cld函数中调用wait获得子进程退出时的状态，并且此时wait不会阻塞。 当sig_cld函数执行完后，父进程又继续做自己的事情。
*/
	if (signal(SIGCLD, sig_cld) == SIG_ERR)
		perror("signal error");
	if ((pid = fork()) < 0) 
	{
		perror("fork error");
	} else if (pid == 0) 
	{	
		/* child */
		sleep(10);
		_exit(0);
	}

	pause();	/* parent */
	exit(0);
}

static void sig_cld(int signo)	/* interrupts pause() */
{
	pid_t	pid;
	int		status;

	printf("SIGCLD received\n");

	if (signal(SIGCLD, sig_cld) == SIG_ERR)	
	{
		/* reestablish handler */
		perror("signal error");
	}

	if ((pid = wait(&status)) < 0)
	{			
		/* fetch child status */
		perror("wait error");
	}

	printf("pid = %d\n", pid);
}
