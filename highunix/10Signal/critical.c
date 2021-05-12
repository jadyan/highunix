#include "apue.h"

/*
	ctrl-c 发送 SIGINT 信号给前台进程组中的所有进程。常用于终止正在运行的程序。
	ctrl-z 发送 SIGTSTP 信号给前台进程组中的所有进程，常用于挂起一个进程。
	ctrl-d 不是发送信号，而是表示一个特殊的二进制值，表示 EOF。
	ctrl-\ 发送 SIGQUIT 信号给前台进程组中的所有进程，终止前台进程并生成 core 文件。
*/

static void	sig_quit(int);

int main(void)
{
	sigset_t	newmask, oldmask, pendmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
		err_sys("can't catch SIGQUIT");

	/*
	 * Block SIGQUIT and save current signal mask.
	 该函数的作用是将信号集初始化为空。
	 */
	sigemptyset(&newmask);
	
	/*
		该函数的作用是把信号signo (SIGQUIT)添加到信号集set中，成功时返回0，失败时返回-1。
		SIGQUIT 但由QUIT字符(通常是Ctrl+\)来控制. 进程在因收到SIGQUIT退出时会产生core文件, 在这个意义上类似于一个程序错误信号。
	*/
	sigaddset(&newmask, SIGQUIT);
	
	/*
		int sigpromask(int how, const sigset_t *set, sigset_t *oset);
		该函数可以根据参数指定的方法修改进程的信号屏蔽字。新的信号屏蔽字由参数set（非空）指定，
		而原先的信号屏蔽字将保存在oset（非空）中。
		如果set为空(NULL)，则how没有意义，但此时调用该函数，如果oset不为空，则把当前信号屏蔽字保存到oset中。
		信号屏蔽
	*/
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)   // SIG_BLOCK 追加 （添加 SIGQUIT）
		err_sys("SIG_BLOCK error");

	printf("sleep5 pre\n");
	/*
			在此期间所产生的退出信号SIGQUIT都被阻塞，不递送至该进程，直到该信号不再被阻塞。在5秒休眠结束后，检查该信号是否是未决的，然后将SIGQUIT设置为不再阻塞。
	*/
	sleep(5);	/* SIGQUIT here will remain pending */
	printf("sleep5 end\n");
	

	/*
		int sigpending(sigset_t *set);
		该函数的作用是将被阻塞的信号中停留在待处理状态的一组信号写到参数set指向的信号集中，成功调用返回0，否则返回-1，并设置errno表明错误原因
		sigpending函数返回信号集，其中的各个信号对于调用进程是阻塞的而不能递送，因而也一定是当前未决的。该信号集通过set参数返回。
		（这些信号是已经产生的信号，但因为信号屏蔽字中对其设置了屏蔽位，从而被阻塞，不能递送给进程的那些信号。注意sigpending返回的信号集与信号屏蔽字的区别。从集合角度来讲，此信号集是当前信号屏蔽字的子集。）
		
	*/
	if (sigpending(&pendmask) < 0)
	{
		err_sys("sigpending error");
	}
	/*
		5、int sigismember(sigset_t *set, int signo);
		该函数的作用是判断给定的信号signo是否是信号集中的一个成员，如果是返回1，如果不是，返回0，如果给定的信号无效，返回-1
	*/
	if (sigismember(&pendmask, SIGQUIT))
	{
		printf("\nSIGQUIT pending\n");
	}

	printf("sigprocmask oldmask pre\n");
	printf("屏蔽信号回滚之前\n");
	/*
	 * Restore signal mask which unblocks SIGQUIT.
	 */
#if 1
	/*
	恢复之前的屏蔽信号 屏蔽信号不包括SIGQUIT， 才能处理SIGQUIT
	*/
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		err_sys("SIG_SETMASK error");
	}
#endif
	printf("屏蔽信号回滚之后\n");
	printf("sigprocmask oldmask end\n");
	
	printf("SIGQUIT unblocked\n");

	sleep(5);	/* SIGQUIT here will terminate with core file */
	exit(0);

}

static void
sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	/*
		SIG_DFL	默认处理：信号由该特定信号的默认操作处理。
		SIG_IGN	忽略信号：忽略信号。
	*/
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		err_sys("can't reset SIGQUIT");
	}
}
