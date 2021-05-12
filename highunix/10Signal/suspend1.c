#if 0
sigset_t　　newmask, oldmask;
sigemptyset(&newmask);
sigaddset(&newmask, SIGINT);
/* block SIGINT and save current signal mask */
if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) <0)
    err_sys("SIG_BLOCK error");
/* critical region of code */
/* restore signal mask, which unblocks SIGINT */
if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    err_sys("SIG_SETMASK error");
/* window is open */
pause(); /* wait for signal to occur continue processing */
如果在信号阻塞时，产生了信号，那么该信号的传递就被推迟直到对它解除了阻塞。

对应用程序而言，该信号好像发生在解除对SIGINT的阻塞和pause之间（取决于内核如何实现信号）。如果发生了这种情况，或者如果在解除阻塞时刻和 pause 之间确实发生了信号，那么就会产生问题
因为可能不会再见到该信号，所以从这种意义上讲，在此时间窗口中发生的信号丢失了，这样就使得pause永远阻塞。这是早期的不可靠信号机制的另一个问题。
	https://blog.csdn.net/big_bit/article/details/51346309
	
	更改进程的信号屏蔽字可以阻塞所选择的信号，或解除对它们的阻塞。使用这种技术可以保护不希望由信号中断的代码临界区。
		如果希望对一个信号解除阻塞，然后pause以等待以前被阻塞的信号发生，则又将如何呢？假定信号是SIGINT
	如果在信号阻塞时，产生了信号，那么该信号的传递就被推迟直到对它解除了阻塞。

	对应用程序而言，该信号好像发生在解除对SIGINT的阻塞和pause之间（取决于内核如何实现信号）。
	如果发生了这种情况，或者如果在解除阻塞时刻和 pause 之间确实发生了信号，那么就会产生问题。因为可能不会再见到该信号，所以从这种意义上讲，在此时间窗口中发生的信号丢失了，这样就使得pause永远阻塞。这是早期的不可靠信号机制的另一个问题。

https://www.shuzhiduo.com/A/ke5j01bXzr/
#endif

#include "apue.h"

static void	sig_int(int);

int main(void)
{
	sigset_t	newmask, oldmask, waitmask;

	pr_mask("program start: ");

	if (signal(SIGINT, sig_int) == SIG_ERR)
	{
		err_sys("signal(SIGINT) error");
	}
	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);

	/*
	 * Block SIGINT and save current signal mask.
	 */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");

	/*
		临界区 从这里到 sigsuspend 函数
	*/
	/*
	 * Critical region of code.
	 */
	pr_mask("in critical region: ");

	/*
	 * Pause, allowing all signals except SIGUSR1.
	 */
	 /*
	 	 将进程的信号屏蔽字设置为由sigmask指向的值。在捕捉到一个信号或发生了一个会终止该进程的信号之前，该进程被挂起。
	 	 如果捕捉到一个信号而且从该信号处理程序返回，则sigsuspend返回，并且将该进程的信号屏蔽字设置为调用sigsuspend之前的值。
	 	 waitmask 代替 newmask
	 	 当sigsuspend返回时，它将信号屏蔽字设置为调用它之前的值。在本例中，SIGINT信号将被阻塞。因此将信号屏蔽恢复为之前保存的值（oldmask）
	 	 
	 	 waitmask当前组赛的集合
	 */
	if (sigsuspend(&waitmask) != -1)
		err_sys("sigsuspend error");

	pr_mask("after return from sigsuspend: ");

	/*
	 * Reset signal mask which unblocks SIGINT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");

	/*
	 * And continue processing ...
	 */
	pr_mask("program exit: ");

	exit(0);
}

static void sig_int(int signo)
{
	pr_mask("\nin sig_int: ");
}
