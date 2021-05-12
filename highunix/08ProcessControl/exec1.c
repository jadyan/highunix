#include "apue.h"
#include <sys/wait.h>

/*
 * 204 页
 * */

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

/*
	execl是用来执行参数path字符串所代表的文件路径，并为新程序复制最后一个参数所指示的环境变量。接下来的参数代表执行该文件时传递过去的argv[0]、argv[1]……，最后一个参数必须用空指针(NULL)作结束。
	
	execlp()会从PATH 环境变量所指的目录中查找符合参数file的文件名，找到后便执行该文件，
	然后将第二个以后的参数当做该文件的argv[0]、argv[1]……，
	最后一个参数必须用空指针(NULL)作结束。
	如果用常数0来表示一个空指针，则必须将它强制转换为一个字符指针，否则它将解释为整形参数，
	如果一个整形数的长度与char * 的长度不同，那么exec函数的实际参数就将出错。如果函数调用成功,进程自己的执行代码就会变成加载程序的代码,execlp()后边的代码也就不会执行了.
*/
int main(int argc , char ** argv)
{
	pid_t	pid;

	if ((pid = fork()) < 0) 
	{
		err_sys("fork error");
	}
	else if (pid == 0)
	{	
		/* specify pathname, specify environment */
		//if (execle("/home/sar/bin/echoall", "echoall", "myarg1", "MY ARG2", (char *)0, env_init) < 0)
		//execle 必须全路径
		//if (execle("/home/jadyan/code/book/highunix/08ProcessControl/echoall", "echoall", "myarg1",	"MY ARG2", (char *)0, env_init) < 0)
		if (execle("/home/jadyan/code/book/highunix/08ProcessControl/echoall", "echoall", "myarg1",	"MY ARG2", (char *)0, env_init) < 0)
		{
			err_sys("execle error");
		}
	}
	//父进程等待子进程终止
	if (waitpid(pid, NULL, 0) < 0)
	{
		err_sys("wait error");
	}

	if ((pid = fork()) < 0) 
	{
		err_sys("fork error");
	} 
	else if (pid == 0) 
	{	/* specify filename, inherit environment */
		//if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
		/*然后将第二个以后的参数当做该文件的argv[0]、argv[1]……，*/
		if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
		{
			err_sys("execlp error");
		}
	}
	exit(0);
}
