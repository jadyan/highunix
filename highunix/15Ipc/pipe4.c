#include "apue.h"




//	缓冲器为暂时置放输出或输入资料的内存。
//	//缓冲器内资料自储存设备（如硬盘）来，放置在缓冲器中，须待机送至CPU或其他运算设备。
//	//缓冲区(buffer)这个中文译意源自当计算机的高速部件与低速部件通讯时,必须将高速部件的输出暂存到某处,以保证高速部件与低速部件相吻合. 后来这个意思被扩展了,成为"临时存贮区"的意思。
//	协同进程
//	page 445
//	使用方法：
//		./pipe4
//			输入：13 14
//			输出 ：27
//	https://blog.csdn.net/astrotycoon/article/details/45874843
//第一步， 创建两个管道，fd1[2]和fd2[2]（父进程在fd1[1]写入数据，再从fd2[0]读出数据）。
//第二步， 调用fork()创建子进程。
//第三步， 在子进程中，关闭fd1[1], fd2[0]，并调用dup2使协同进程的标准输入连接到fd1[0], 标准输出连接到fd2[1]，这样就将两个管道连接起来了。
//		   再在子进程中调用execl调用编写的协同处理程序（这里的协同程序做为一般的程序编写即可，从标准输入读入数据，处理后输出到标准输出）。
//第四步， 在父进程中，关闭fd1[0], fd2[1]，将协同进程需要处理的数据写入fd1[1], 再从fd2[0]读出协同进程的输出即可

//在程序中创建了两个管道，父、子进程各自关闭它们不需要使用的端口。两个管道一个用作协同进程的标准输入，另一个则用作它的标准输出。子进程调用dup2使管道描述符移至其标准输入和标准输出，然后调用execl。

static void	sig_pipe(int);		/* our signal handler */



int
main(void)
{
	int		n, fd1[2], fd2[2];
	pid_t	pid;
	char	line[MAXLINE];

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
	{
		err_sys("signal error");
	}

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
	{
		err_sys("pipe error");
	}

	if ((pid = fork()) < 0) 
	{
		err_sys("fork error");
	}
	else if (pid > 0) 
	{							/* parent */
		close(fd1[0]); //读
		close(fd2[1]); //写

		while (fgets(line, MAXLINE, stdin) != NULL) 
		{
			n = strlen(line);
			if (write(fd1[1], line, n) != n)
			{
				err_sys("write error to pipe");
			}
			if ((n = read(fd2[0], line, MAXLINE)) < 0)
			{
				err_sys("read error from pipe");
			}
			if (n == 0) 
			{
				err_msg("child closed pipe");
				break;
			}
			line[n] = 0;	/* null terminate */
			if (fputs(line, stdout) == EOF)
			{
				err_sys("fputs error");
			}
		}

		if (ferror(stdin))
		{
			err_sys("fgets error on stdin");
		}
		exit(0);
	} else {									/* child */
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) 
		{
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
			{
				err_sys("dup2 error to stdin");
			}
			close(fd1[0]);
		}

		if (fd2[1] != STDOUT_FILENO) 
		{
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
			{
				err_sys("dup2 error to stdout");
			}
			close(fd2[1]);
		}
		if (execl("./add2", "add2", (char *)0) < 0)
			err_sys("execl error");
	}
	exit(0);
}

static void
sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}
