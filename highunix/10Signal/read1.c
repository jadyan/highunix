#include "apue.h"
#include <stdio.h>
#include <stdlib.h>

/*
	271 页
	例如，程序中有一个读低速设备的可能阻塞的操作（见 10.5 节），我们希望超过一定时间量后就停止执行该操作。图10-10实现了这一点，它从标准输入读一行，然后将其写到标准输出上。
	：在第一次alarm 调用和read调用之间有一个竞争条件。如果内核在这两个函数调用之间使进程阻塞，不能占用处理机运行，而其时间长度又超过闹钟时间，则read可能永远阻塞。
	  大多数这种类型的操作使用较长的闹钟时间，例如1分钟或更长一点，使这种问题不会发生，但无论如何这是一个竞争条件。
     如果系统调用是自动重启动的，则当从SIGALRM信号处理程序返回时，read并不被中断。在这种情形下，设置时间限制不起作用。
     
     
     参数seconds的值是产生信号SIGALRM需要经过的时钟秒数。当这一时刻到达时，信号由内核产生，由于进程调度的延迟，所以进程得到控制从而能够处理该信号还需要一个时间间隔。


	已设置 再替代  每个进程只能有一个闹钟时间。如果在调用alarm时，之前已为该进程注册的闹钟时间还没有超时，则该闹钟时间的余留值作为本次alarm函数调用的值返回。以前注册的闹钟时间则被新值代替。

	如果有以前注册的尚未超过的闹钟时间，而且本次调用的seconds值是0，则取消以前的闹钟时间，其余留值仍作为alarm函数的返回值。

	虽然 SIGALRM 的默认动作是终止进程，但是大多数使用闹钟的进程捕捉此信号。如果此时进程要终止，则在终止之前它可以执行所需的清理操作。
		如果我们想捕捉 SIGALRM 信号，则必须在调用 alarm 之前安装该信号的处理程序。如果我们先调用alarm，然后在我们能够安装SIGALRM处理程序之前已接到该信号，那么进程将终止。
*/
static void	sig_alrm(int);

int
main(void)
{
	int		n;
	char	line[MAXLINE];

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		err_sys("signal(SIGALRM) error");

	alarm(10);
	if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
		err_sys("read error");
	alarm(0);

	write(STDOUT_FILENO, line, n);
	exit(0);
}

static void sig_alrm(int signo)
{
	/* nothing to do, just return to interrupt the read */
}
