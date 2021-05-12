#include "apue.h"
#include <sys/wait.h>



/*
//	${var} 就是 $var，和周围隔得开。
//	${var-val} 如果 var 未定义，用 val 顶上
//	${var:-val} 如果 var 空，顶上
//	${var=val} 如果 var空，顶上并且赋值为 val

//	${var1:-var2}
//	这种结构执行过程：如果变量var1有定义且不为空时，返回$var1，如果var1未定义或者为空，则返回‘-’后面的内容
*/


//函数说明：popen()会调用fork()产生子进程，然后从子进程中调用/bin/sh -c 来执行参数command 的指令。
//参数type 可使用 "r"代表读取，"w"代表写入。依照此type 值，popen()会建立管道连到子进程的标准输出设备或标准输入设备，然后返回一个文件指针。
//随后进程便可利用此文件指针来读取子进程的输出设备或是写入到子进程的标准输入设备中。

#define	PAGER	"${PAGER:-more}" /* environment variable, or default */

int
main(int argc, char *argv[])
{
	char	line[MAXLINE];
	FILE	*fpin, *fpout;

	if (argc != 2)
	{
		err_quit("usage: a.out <pathname>");
	}
	if ((fpin = fopen(argv[1], "r")) == NULL)
	{
		err_sys("can't open %s", argv[1]);
	}

	if ((fpout = popen(PAGER, "w")) == NULL)
	{
		err_sys("popen error");
	}

	/* copy argv[1] to pager */
	while (fgets(line, MAXLINE, fpin) != NULL) 
	{
		if (fputs(line, fpout) == EOF)
			err_sys("fputs error to pipe");
	}
	if (ferror(fpin))
	{
		err_sys("fgets error");
	}
	//结束后台程序，并关闭管道  
	if (pclose(fpout) == -1)
	{
		err_sys("pclose error");
	}

	exit(0);
}
