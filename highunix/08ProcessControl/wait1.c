


#include "apue.h"
#include <sys/wait.h>

/*
 * 
 * 父进程一旦调用了wait就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，
 * 如果让它找到了这样一个已经变成僵尸的子进程，
 * wait就会收集这个子进程的信息，并把它彻底销毁后返回；
 * 如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。
 * */

int
main(void)
{
	pid_t	pid;
	int		status;

/*
 *
 1. 如果其所有子进程都还在运行，则阻塞 
 2. 如果一个子进程已终止，整等待父进程获取其终止状态，则取得该子进程的终止状态立即返回。 
 3. 如果它没有收到任何子进程，则立即出错返回
 *
 * */
 char *info = "***********************************************\n"
              "pid_t wait(int *stattloc)\n"
              "pid_t waitpid(pid_t pid, int *staloc, int options); \n"
              "一个子进程终止前，wait使其调用者阻塞，而waitpid有一选项，可使调用者不阻塞。\n"
              "waitpid并不等待在其调用之后的第一个终止子进程，它有若干个选项，可以控制它所等待的进程。\n"
              "其中staloc是一个指向终止状态的指针，若不需要终止状态则可以设置为空。\n"
              "wait 函数说明\n"
              "1. 如果其所有子进程都还在运行，则阻塞 \n"
              "2. 如果一个子进程已终止，整等待父进程获取其终止状态，则取得该子进程的终止状态立即返回。\n"
              "3. 如果它没有收到任何子进程，则立即出错返回\n"
              "page 190\n"
              "************************************************\n";
 printf("%s",info);
 
printf("example 1:");
	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if (pid == 0)	
    {
	    /* child */
		exit(7);
		//while(1)
		//{
		//	printf("子进程run\n");
		//	sleep(2);
		//}
    }

	//一个子进程终止前，wait使其调用者阻塞(父进程阻塞)
	if (wait(&status) != pid)		/* wait for child */
    {
		err_sys("wait error");
	}
	
	pr_exit(status);				/* and print its status */


printf("example 2:");
	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if (pid == 0)				/* child */
	{
		/*  abort 异常终止一个进程 */
		abort();					/* generates SIGABRT */
	}
	if (wait(&status) != pid)		/* wait for child */	
	{
		err_sys("wait error");
	}
	pr_exit(status);				/* and print its status */


printf("example 3:");
	if ((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if (pid == 0)				/* child */
	{
		status /= 0;				/* divide by 0 generates SIGFPE */
	}

	if (wait(&status) != pid)		/* wait for child */
	{
		err_sys("wait error");
	}

	pr_exit(status);				/* and print its status */

	exit(0);
}
