#include "apue.h"
#include <sys/times.h>

/* 
 *第8章
	8.17 进程时间
	执行方式
		./times1 "sleep 5" "date" "man bash>>/dev/null" 
		
		clock_t times(struct tms *buf);

	//tms结构体如下:
	//strace tms{
	// clock_t tms_utime;
	// clock_t tms_stime;
	// clock_t tms_cutime;
	// clock_t tms_cstime;
	//}
	//
	//注释:
	//tms_utime记录的是进程执行用户代码的时间.     /* User CPU time.  用户程序 CPU 时间*/ 
	//tms_stime记录的是进程执行内核代码的时间.     /* System CPU time. 系统调用所耗费的 CPU 时间 */ 
	//tms_cutime记录的是子进程执行用户代码的时间.  /* User CPU time of dead children. 已死掉子进程的 CPU 时间*/ 
	//tms_cstime记录的是子进程执行内核代码的时间.  /* System CPU time of dead children.  已死掉子进程所耗费的系统调用 CPU 时间*/ 
		
		

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

int main(int argc, char *argv[])
{
	int		i;
	setbuf(stdout, NULL);
	for (i = 1; i < argc; i++)
	{
		do_cmd(argv[i]);	/* once for each command-line arg */
	}
	exit(0);
}

static void do_cmd(char *cmd)		/* execute and time the "cmd" */
{
	struct tms	tmsstart, tmsend;
	clock_t		start, end;
	int			status;

	printf("\ncommand: %s\n", cmd);

	/*进程运行到此时的系统时钟数(总的)*/ 
	if ((start = times(&tmsstart)) == -1)	/* starting values */
	{
		err_sys("times error");
	}

//因为system会调用fork exec waitid 所以会显示子进程执行时间
	if ((status = system(cmd)) < 0)			/* execute command */
	{
		err_sys("system() error");
	}

	/*进程到此时的系统时钟数*/ 
	if ((end = times(&tmsend)) == -1)		/* ending values */
	{
		err_sys("times error");
	}

	pr_times(end-start, &tmsstart, &tmsend);
	pr_exit(status);
}

static void pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
	static long		clktck = 0;

	if (clktck == 0)
	{
		/*获取系统时钟(1秒里有多少个 系统时钟数)*/ 
		if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
		{
			err_sys("sysconf error");
		}
	}

	printf("  real:  %7.2f\n",        real / (double) clktck);
	printf("  user[命令执行完成花费的系统CPU时间]:  %7.2f\n",       (tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);     //记录的是进程执行用户代码的时间. 命令执行完成花费的系统CPU时间，即命令在用户态中执行时的总和
	printf("  sys[录的是进程执行内核代码的时间]:   %7.2f\n",       (tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);     //记录的是进程执行内核代码的时间.
	printf("  child user[子进程执行用户代码的时间]:  %7.2f\n", (tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);   //记录的是子进程执行用户代码的时间.
	printf("  child sys[子进程执行内核代码的时间]:   %7.2f\n", (tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);   //记录的是子进程执行内核代码的时间.
}
