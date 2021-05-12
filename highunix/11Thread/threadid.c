#include "apue.h"
#include <pthread.h>

/*
 * 第11章
 *  11.4 线程创建
 * */
pthread_t ntid;

void printids(const char *s)
{
	pid_t		pid;
	pthread_t	tid;

	pid = getpid();
	//返回自己的线程ID
	tid = pthread_self();
	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void * thr_fn(void *arg)
{
	//打印线程ID与进程主ID
	printids("new thread: ");
	return((void *)0);
}

int
main(void)
{
	int		err;

	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (err != 0)
	{
		err_exit(err, "can't create thread");
	}
	//ntid 主线程会把新的线程ID 放到 ntid
	printf("%s tid %lu (0x%lx)\n", "pthread_create 参数 返回值 ntid", (unsigned long)ntid, (unsigned long)ntid);
	//打印主线程ID与进程主ID
	printids("main thread:");
	sleep(1);
	exit(0);
}
