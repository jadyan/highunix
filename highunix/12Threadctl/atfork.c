#include "apue.h"
#include <pthread.h>

/*
 * 12.9
 *线程与fork
page 368
 * */
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void prepare(void)
{
	int err;

	printf("preparing locks...\n");
	if ((err = pthread_mutex_lock(&lock1)) != 0)
	{
		err_cont(err, "can't lock lock1 in prepare handler");
	}
	if ((err = pthread_mutex_lock(&lock2)) != 0)
	{
		err_cont(err, "can't lock lock2 in prepare handler");
	}
}

void parent(void)
{
	int err;

	printf("parent unlocking locks...\n");
	if ((err = pthread_mutex_unlock(&lock1)) != 0)
	{
		err_cont(err, "can't unlock lock1 in parent handler");
	}
		if ((err = pthread_mutex_unlock(&lock2)) != 0)
	{
		err_cont(err, "can't unlock lock2 in parent handler");
	}
}

void child(void)
{
	int err;

	printf("child unlocking locks...\n");
	if ((err = pthread_mutex_unlock(&lock1)) != 0)
	{
		err_cont(err, "can't unlock lock1 in child handler");
	}
	if ((err = pthread_mutex_unlock(&lock2)) != 0)
	{
		err_cont(err, "can't unlock lock2 in child handler");
	}
}

void * thr_fn(void *arg)
{
	printf("thread started...\n");
	/*
	定义函数：int pause(void);
	函数说明：pause()会令目前的进程暂停(进入睡眠状态), 直到被信号(signal)所中断.
	*/
	pause();
	return(0);
}

/*
	子进程通过继承整个地址空间的副本，还从父进程那儿继承了每个互斥量、读写锁和条件变量的状态。
	如果父进程包含一个以上的线程，子进程在fork返回以后，如果紧接着不是马上调用exec的话，就需要清理锁状态。
	在子进程内部，只存在一个线程，它是由父进程中调用fork的线程的副本构成的。如果父进程中的线程占有锁，子进程将同样占有这些锁。
	问题是子进程并不包含占有锁的线程的副本，所以子进程没有办法知道它占有了哪些锁、需要释放哪些锁。
*/

int main(void)
{
	int			err;
	pid_t		pid;
	pthread_t	tid;

/*
	prepare fork处理程序由父进程在fork创处理程序由父进程在fork创建子进程前调用。这个fork处理程序的任务是获取父进程定义的所有锁。(fork 前 父进程)
	parentfork处理程序是在fork 创建子进程以后、返回之前在父进程上下文中调用的。这个fork处理程序的任务是对prepare fork处理程序获取的所有锁进行解锁。(fork中 父进程)
	child fork处理程序在fork返回之前在子进程上下文中调用。child fork处理程序也必须释放prepare fork处理程序获取的所有锁。 (fork中 紫禁城进程)
*/
	if ((err = pthread_atfork(prepare, parent, child)) != 0)
	{
		err_exit(err, "can't install fork handlers");
	}
	if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
		err_exit(err, "can't create thread");

	sleep(5);
	printf("parent about to fork...\n");

	if ((pid = fork()) < 0)
	{
		err_quit("fork failed");
	}
	else if (pid == 0)	/* child */
	{
		printf("child returned from fork\n");
	}
	else		/* parent */
	{
		printf("parent returned from fork\n");
	}
	exit(0);
}
