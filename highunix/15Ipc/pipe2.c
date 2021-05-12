#include "apue.h"
#include <sys/wait.h>

#define	DEF_PAGER	"/bin/more"		/* default pager program */

//page 431

//	#include <unistd.h>
//	int dup2(int oldfd, int newfd);
//	用参数newfd指定新文件描述符的数值 若参数newfd已经被程序使用，则系统就会将newfd所指的文件关闭，
//	若newfd等于oldfd，则返回newfd,而不关闭newfd所指的文件。dup2所复制的文件描述符与原来的文件描述符共享各种文件状态。共享所有的锁定，读写位置和各项权限或flags等




int
main(int argc, char *argv[])
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	*pager, *argv0;
	char	line[MAXLINE];
	FILE	*fp;

	if (argc != 2)
	{
		err_quit("usage: a.out <pathname>");
	}

	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		err_sys("can't open %s", argv[1]);
	}
	if (pipe(fd) < 0)
	{
		err_sys("pipe error");
	}

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid > 0){								/* parent */
		close(fd[0]);		/* close read end */

		/* parent copies argv[1] to pipe */
		while (fgets(line, MAXLINE, fp) != NULL) 
		{
			n = strlen(line);
			if (write(fd[1], line, n) != n)
				err_sys("write error to pipe");
		}
		if (ferror(fp))
		{
			err_sys("fgets error");
		}

		close(fd[1]);	/* close write end of pipe for reader */

		if (waitpid(pid, NULL, 0) < 0)
		{
			err_sys("waitpid error");
		}
		exit(0);
	} else {										/* child */
		close(fd[1]);	/* close write end */
		if (fd[0] != STDIN_FILENO) 
		{
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
				err_sys("dup2 error to stdin");
			close(fd[0]);	/* don't need this after dup2 */
		}

		/* get arguments for execl() */
		if ((pager = getenv("PAGER")) == NULL)
		{
			pager = DEF_PAGER;
		}
		if ((argv0 = strrchr(pager, '/')) != NULL)
		{
			argv0++;		/* step past rightmost slash */
			printf("^^^^^^argv0 = %s \n", argv0);
		}
		else
		{
			argv0 = pager;	/* no slash in pager */
			printf("$$$$$$$$argv0 = %s \n", argv0);
		}
		
		if (execl(pager, argv0, (char *)0) < 0)
		{
			err_sys("execl error for %s", pager);
		}
	}
	exit(0);
}
