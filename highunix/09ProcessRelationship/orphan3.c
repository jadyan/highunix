//#include "apue.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

/*
 *进程关系
 * page 245
 * 孤儿进程组
 *孤儿进程组不可以获得终端，这是为了保证控制进程死掉后他的终端可以安全分配给新session。posix要求向新孤儿进程组中停止状态的进程（GNU说是给孤儿进程组里所有进程发消息，而APUE中文版说是给停止状态的进程发消息）发送SIGHUP（挂起）信号和SIGCONT（继续）信号。首先处理SIGHUP信号，系统默认处理是终止进程，然而也可以另行处理这样进程会继续执行，但任不可以再获得终端
 * */


/*
	//孤儿进程组：该进程组中每一个进程的父进程都属于另一个session.
    //
	//怎样创建孤儿进程组？
    //
	//fork()后，子进程继承父进程的gid,然后父进程退出，那么子进程变成了孤儿进程，其所在的进程组也变成了孤儿进程组。
    //
	//特性1：父进程终止后，进程组成为了孤儿进程组。那么新的孤儿进程组中处于停止(stopped)状态的每一个进程都会收到挂断(SIGHUP)信号，接着又收到继续(SIGCONT)信号。
    //
	//也就是说，进程组成为孤儿进程组后，孤儿进程组中的状态为stopped的进程会被激活。前提是需要对SIGHUP信号自处理，对挂断信号系统默认的动作是终止进程。
    //
	//特性2：孤儿进程组是后台进程组，且没有控制终端
    //
	//特性3：孤儿进程组去读控制终端时，read返回出错并将errno设置为EIO。
*/


/*
	因为父进程终止或崩溃都会导致对应子进程成为孤儿进程，所以也无法预料一个子进程执行期间是否会被“遗弃”。
	有鉴于此，多数类UNIX系统都引入了进程组以防止产生孤儿进程：在父进程终止后，用户的Shell会将父进程所在进程组标为“孤儿进程组”，
	并向终止的进程下属所有子进程发出SIGHUP信号，以试图结束其运行，如此避免子进程继续以“孤儿进程”的身份运行[2]。
*/
static void sig_hup(int signo)
{
	printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

static void pr_ids(char *name)
{
	/*
		getpid()  进程ID
		getppid() 父进程ID
		getpgrp() 用来取得目前进程所属的组识别码。此函数相当于调用getpgid（0）；返回目前进程所属的组识别码。
		tcgetpgrp(int i)函数tcgetpgrp返回和在filedes上打开的终端相关联的前台进程组的进程组ID。
				如果进程有一个控制终端，进程可以调用tcsetpgrp来设置前台进程组ID给pgrpid。pgrpid的值必须是相同会话里的一个进程组的进程组ID，filedes必须引用会话的控制终端
	*/
	printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
	fflush(stdout);
}


/*
	//1：首先让父进程休眠5秒钟，让子进程在父进程终止之前运行。
	//2：子进程为SIGHUP信号建立信号处理程序，用于观察SIGHUP信号是否已经发送到子进程。
	//3：子进程用kill函数向自身发送SIGTSTP信号，模拟用终端停止一个前台作业。
	//4：父进程终止时，该子进程成为了一个孤儿进程，ppid=1。
	//5：现在，子进程成为一个孤儿进程组的成员。
	//6：父进程停止后，进程组成为了孤儿进程组，父进程会向新的孤儿进程组中处于停止状态的每个进程发送SIGHUP信号，接着又向其发送SIGCONT信号。
	//7：在处理了SIGHUP信号之后，子进程继续。对SIGHUP信号的默认动作是终止该进程，所以必须提供一个信号处理程序以捕捉该信号。
*/
int main(void)
{
	char	c;
	pid_t	pid;

	pr_ids("parent");
	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	} else if (pid > 0) 
	{
		/* parent */
		sleep(5);		/* sleep to let child stop itself */
	}
	else 
	{		
		/* child */	
		pr_ids("child");
		
		/*
			SIGHUP duiying 处理程序
		*/
		signal(SIGHUP, sig_hup);	/* establish signal handler */
		/*
			 kill(getpid(), SIGTSTP)并不是要child杀死自己，而是暂停前台作业。
		*/
		kill(getpid(), SIGTSTP);	/* stop ourself */
		pr_ids("child");	/* prints only if we're continued */
		printf("STDIN_FILENO =%d \n", STDIN_FILENO);
		if (read(STDIN_FILENO, &c, 1) != 1)
		{
			printf("read error %d on controlling TTY\n", errno);
		}
	}
	exit(0);
}
