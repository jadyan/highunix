#include "apue.h"
#include <sys/wait.h>
/*
 * 213页
 */
 
 /*
 system 在实现中 调用了fork、exec、waitpid 
  如果无法启动shell运行命令，system将返回127；
  出现不能执行system调用的其他错误时返回-1。
  如果system能够顺利执行，返回那个命令的退出码。
*/
int main(void)
{
	int		status;

	if ((status = system("date")) < 0)
	{
		err_sys("system() error");
	}
	printf("__________________status=%d__________________\n", status);
	pr_exit(status);

	if ((status = system("nosuchcommand")) < 0)
	{
		err_sys("system() error");
	}
	printf("__________________status=%d__________________\n", status);
	pr_exit(status);

	if ((status = system("who; exit 44")) < 0)
	{
		err_sys("system() error");
	}
	printf("__________________status=%d__________________\n", status);
	pr_exit(status);

	exit(0);
}
