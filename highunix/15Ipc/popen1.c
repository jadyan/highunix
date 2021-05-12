#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
	char	line[MAXLINE];
	FILE	*fpin;

    //创建管道，并将其链接到后台程序中  
    //popen() 函数通过创建一个管道，调用fork产生一个子进程，  
    //执行一个shell以运行命令来开启一个进程。 
	if ((fpin = popen("./myuclc", "r")) == NULL)
	{
		err_sys("popen error");
	}
	for ( ; ; ) 
	{
		//输出提示（因为不含回车符，所以要手动刷新  
		//fputs("prompt> ", stdout);
		fputs("jadyan> ", stdout);
		fflush(stdout);
		//向后台程序输入在终端输入的字符串  
		if (fgets(line, MAXLINE, fpin) == NULL)	/* read from pipe */
			break;
		//将后台程序的输出输出到终端  
		if (fputs(line, stdout) == EOF)
			err_sys("fputs error to pipe");
	}
	if (pclose(fpin) == -1)
		err_sys("pclose error");
	putchar('\n');
	exit(0);
}
