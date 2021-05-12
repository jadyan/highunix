#include <stdio.h>
#include <stdlib.h>
#include	<signal.h>
#include	<unistd.h>

/*
 *alarm函数
 page 268
 * */
 //https://anmingyu11.gitbooks.io/unix/content/di-shi-zhang-xin-hao/1010-han-shu-alarm-he-pause.html
 
/*
	pause函数使调用进程挂起直至捕捉到一个信号。
	#include <unistd.h>
	int pause(void);
	//返回值：−1，errno设置为EINTR
	只有执行了一个信号处理程序并从其返回时，pause才返回。在这种情况下，pause返回−1， errno设置为EINTR。
*/
static void sig_alrm(int signo)
{
	/* nothing to do, just return to wake up the pause */
	printf("捕捉SIGALRM信号\n");
}

unsigned int sleep1(unsigned int seconds)
{
#if 0
	/*
	如果忽略或不捕捉此信号，则其默认动作是终止调用该alarm函数的进程。
	*/
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
	{
		return(seconds);
	}
#endif
	alarm(seconds);		/* start the timer */
	/*
	只有执行了一个信号处理程序并从其返回时，pause才返回。在这种情况下，pause返回−1， errno设置为EINTR。
	*/
	pause();			/* next caught signal wakes us up */
	return(alarm(0));	/* turn off timer, return unslept time */
}


int main(int argc, char **argv)
{
	sleep1(5);
}
