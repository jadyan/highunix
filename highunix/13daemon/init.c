#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>


/*
	用法：

	#include <sys/resource.h>

	int getrlimit(int resource, struct rlimit *rlim);
	int setrlimit(int resource, const struct rlimit *rlim);
	参数：

	resource：可能的选择有

	RLIMIT_AS //进程的最大虚内存空间，字节为单位。
	RLIMIT_CORE //内核转存文件的最大长度。
	RLIMIT_CPU //最大允许的CPU使用时间，秒为单位。当进程达到软限制，内核将给其发送SIGXCPU信号，这一信号的默认行为是终止进程的执行。然而，可以捕捉信号，处理句柄可将控制返回给主程序。如果进程继续耗费CPU时间，核心会以每秒一次的频率给其发送SIGXCPU信号，直到达到硬限制，那时将给进程发送 SIGKILL信号终止其执行。
	RLIMIT_DATA //进程数据段的最大值。
	RLIMIT_FSIZE //进程可建立的文件的最大长度。如果进程试图超出这一限制时，核心会给其发送SIGXFSZ信号，默认情况下将终止进程的执行。
	RLIMIT_LOCKS //进程可建立的锁和租赁的最大值。
	RLIMIT_MEMLOCK //进程可锁定在内存中的最大数据量，字节为单位。
	RLIMIT_MSGQUEUE //进程可为POSIX消息队列分配的最大字节数。
	RLIMIT_NICE //进程可通过setpriority() 或 nice()调用设置的最大完美值。
	RLIMIT_NOFILE //指定比进程可打开的最大文件描述词大一的值，超出此值，将会产生EMFILE错误。
	RLIMIT_NPROC //用户可拥有的最大进程数。
	RLIMIT_RTPRIO //进程可通过sched_setscheduler 和 sched_setparam设置的最大实时优先级。
	RLIMIT_SIGPENDING //用户可拥有的最大挂起信号数。
	RLIMIT_STACK //最大的进程堆栈，以字节为单位。
*/
void
daemonize(const char *cmd)
{
	int					i, fd0, fd1, fd2;
	pid_t				pid;
	struct rlimit		rl;
	struct sigaction	sa;

	/*
	 * Clear file creation mask.
	 */
	umask(0);

	/*
	 * Get maximum number of file descriptors.
	 *RLIMIT_NOFILE //指定比进程可打开的最大文件描述词大一的值，超出此值，将会产生EMFILE错误。
	 */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_quit("%s: can't get file limit", cmd);

	/*
	 * Become a session leader to lose controlling TTY.
	 */
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if (pid != 0) /* parent */
		exit(0);
	setsid();

	/*
	 * Ensure future opens won't allocate controlling TTYs.
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: can't ignore SIGHUP", cmd);
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if (pid != 0) /* parent */
		exit(0);

	/*
	 * Change the current working directory to the root so
	 * we won't prevent file systems from being unmounted.
	 
	 功 能：更改当前工作目录。
	参 数：Path 目标目录，可以是绝对目录或相对目录。
	返回值：成功返回0 ，失败返回-1
	 */
	if (chdir("/") < 0)
		err_quit("%s: can't change directory to /", cmd);

	/*
	 * Close all open file descriptors
	 限制值是一个“无穷大”值：RLIM_INFINITY.
	 */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	/*
	 * Attach file descriptors 0, 1, and 2 to /dev/null.
	 #include<unistd.h>
	int dup（int fd）；
	int dup2（int fd1，int fd2）；
	两个均为复制一个现存的文件的描述
	两个函数的返回：若成功为新的文件描述，若出错为-1；
	由dup返回的新文件描述符一定是当前可用文件描述中的最小数值。用dup2则可以用fd2参数指定新的描述符数值。如果fd2已经打开，则先关闭。若fd1=fd2，则dup2返回fd2，而不关闭它。通常使用这两个系统调用来重定向一个打开的文件描述符。
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 * Initialize the log file.
	  系统log
	 */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}


int main()
{
     daemonize("test");
     while(1);//守护进程所要干的事情
    
}
