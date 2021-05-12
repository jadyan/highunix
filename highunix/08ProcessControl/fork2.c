#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0) 
	{
		err_sys("fork error");
	} 
	else if (pid == 0) 
	{		
		/* first child */
		if ((pid = fork()) < 0)
		{
			err_sys("fork error");
		}
		else if (pid > 0)
		{
			/*父进程终止*/
			exit(0);	/* parent from second fork == first child */
		}
		/*
		 * We're the second child; our parent becomes init as soon
		 * as our real parent calls exit() in the statement above.
		 * Here's where we'd continue executing, knowing that when
		 * we're done, init will reap our status.
		在UNIX属于中，一个已经终止、但是其父进程尚未对其进行善后处理的进程的进程被称为僵死进程。
		如果一个进程fork一个子进程但是不要它等待子进程终止，也不希望子进程处于僵死状态直到父进程终止，实现这一要求的诀窍：调用fork两次

		 孤儿进程：一个父进程退出，而它的一个或多个子进程还在运行，那么那些子进程将成为孤儿进程。孤儿进程将被init进程(进程号为1)所收养，并由init进程对它们完成状态收集工作。
　　	 僵尸进程：一个进程使用fork创建子进程，如果子进程退出，而父进程并没有调用wait或waitpid获取子进程的状态信息，那么子进程的进程描述符仍然保存在系统中。这种进程称之为僵死进程。
		 */
		sleep(2);
		//结果父进程Id是1 init进程 
		printf("second child, parent pid = %ld\n", (long)getppid());
		exit(0);
	}

	if (waitpid(pid, NULL, 0) != pid)	/* wait for first child */
		err_sys("waitpid error");

	/*
	 * We're the parent (the original process); we continue executing,
	 * knowing that we're not the parent of the second child.
	 */
	exit(0);
}
