#include "apue.h"
#include <sys/wait.h>

/*
 *解释器文件
 * */

int main(int argc, char ** argv)
{
	pid_t	pid;

	if ((pid = fork()) < 0) 
	{
		err_sys("fork error");
	} 
	else if (pid == 0)
	{
		/* child */
		/*
			解释器 第二个参数是可选项testinterp argv[1]是解释器文件中的可选参数（path那么要是awk 话， argv[1] 就是 "-f" ）
			当execl找到一个非机器可执行的可执行文件时，它总是调用/bin/sh来解释执行该文件。
		*/
		if (execl("/home/jadyan/code/book/highunix/08ProcessControl/testinterp", "testinterp", "myarg1", "MY ARG2", (char *)0) < 0)
		{
			err_sys("execl error");
		}
	}

	if (waitpid(pid, NULL, 0) < 0)	/* parent */
	{
		err_sys("waitpid error");
	}
	exit(0);
}
