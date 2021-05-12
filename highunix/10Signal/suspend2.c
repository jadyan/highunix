#include "apue.h"

volatile sig_atomic_t	quitflag;	/* set nonzero by signal handler */

static void sig_int(int signo)	/* one signal handler for SIGINT and SIGQUIT */
{
	if (signo == SIGINT)
	{
		printf("\ninterrupt\n");
	}
	else if (signo == SIGQUIT)
	{
		quitflag = 1;	/* set flag for main loop */
	}
	printf("sig_int函数内 quitflag = %d\n", quitflag);
}

/*
	sigsuspend的另一种应用是等待一个信号处理程序设置一个全局变量。图10-23中的程序用于捕捉中断信号和退出信号，但是希望仅当捕捉到退出信号时，才唤醒主例程。
*/
int main(void)
{
	sigset_t	newmask, oldmask, zeromask;

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");
	if (signal(SIGQUIT, sig_int) == SIG_ERR)
		err_sys("signal(SIGQUIT) error");

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);

	/*
	 * Block SIGQUIT and save current signal mask.
	 */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");

	while (quitflag == 0)
	{
		printf("while quitflag = %d \n", quitflag);
		sigsuspend(&zeromask);    //会组赛在这里
	}

	/*
	 * SIGQUIT has been caught and is now blocked; do whatever.
	 */
	quitflag = 0;

	/*
	 * Reset signal mask which unblocks SIGQUIT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");

	exit(0);
}
