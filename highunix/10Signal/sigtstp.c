#include "apue.h"

#define	BUFFSIZE	1024
/*
	该例子未做深度研究
*/
/*
	https://blog.csdn.net/aisxyz/article/details/84910394
	POSIX.1 认为有以下 6 个与作业控制有关的信号。
	* SIGCHLD：子进程已停止或终止。
	* SIGCONT：如果进程已停止，则使其继续运行，否则忽略。
	* SIGSTOP：停止信号（不能被捕捉或忽略）。
	* SIGTSTP：交互式停止信号。
	* SIGTTIN：后台进程组成员读控制终端。
	* SIGTTOU：后台进程组成员写控制终端。
	除 SIGCHLD 以外，大多数应用程序并不处理这些信号，交互式 shell 则通常会处理这些信号的所有工作。当键入挂起字符（通常是 Ctrl+Z）时，SIGTSTP 就被送至前台进程组的所有进程；
	当通知 shell 在前台或后台恢复运行一个作业时，shell 就向该作业中的所有进程发送 SIGCONT 信号。当对一个停止的进程产生一个 SIGCONT 信号时，该进程就继续，即使该信号是被阻塞或忽略的。
	下面这个程序演示了当一个程序处理作业控制时通常所使用的规范代码序列。该程序只是将其标准输入复制到标准输出，而在信号处理程序中以注释形式给出了管理屏幕的程序（如 VI）所执行的典型操作。
		
		
	$ ./jobCtrlDemo.out
	abcde                                      # 读取输入
	abcde                                      # 输出
	^Zsend SIGTSTP                             # Ctrl-Z 发送 SIGTSTP 信号挂起进程
	[1]+  Stopped                 ./jobCtrlDemo.out
	$ ps                                       # 查看进程 ID
	   PID TTY          TIME CMD
	 23893 pts/1    00:00:00 jobCtrlDemo.out
	 23914 pts/1    00:00:00 ps
	 82002 pts/1    00:00:05 bash
	$ 
	$ kill -SIGCONT 23893                      # 发送 SIGCONT 信号使进程继续运行
	received SIGCONT
	[1]+  Stopped                 ./jobCtrlDemo.out
*/


static void
sig_tstp(int signo)	/* signal handler for SIGTSTP */
{
	sigset_t	mask;

	/* ... move cursor to lower left corner, reset tty mode ... */

	/*
	 * Unblock SIGTSTP, since it's blocked while we're handling it.
	 */
	sigemptyset(&mask);
	sigaddset(&mask, SIGTSTP);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	signal(SIGTSTP, SIG_DFL);	/* reset disposition to default */

	kill(getpid(), SIGTSTP);	/* and send the signal to ourself */

	/* we won't return from the kill until we're continued */

	signal(SIGTSTP, sig_tstp);	/* reestablish signal handler */

	/* ... reset tty mode, redraw screen ... */
}

int
main(void)
{
	int		n;
	char	buf[BUFFSIZE];

	/*
	 * Only catch SIGTSTP if we're running with a job-control shell.
	 */
	if (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
		signal(SIGTSTP, sig_tstp);

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");

	if (n < 0)
		err_sys("read error");

	exit(0);
}
