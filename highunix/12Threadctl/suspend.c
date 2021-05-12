#include "apue.h"
#include <pthread.h>

/*
	https://anmingyu11.gitbooks.io/unix/content/di-shi-er-zhang-xian-cheng-kong-zhi/128-xian-cheng-he-xin-hao.html
 * 线程与信号
 * page 367
 * 12.8
 * */

int			quitflag;	/* set nonzero by thread */
sigset_t	mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void * thr_fn(void *arg)
{
	int err, signo;

	for (;;) {
		/*
			sigwait等待一个或多个信号的出现
			#include <signal.h>
			//int sigwait(const sigset_t *restrict set, int *restrict signop);
			//返回值：若成功，返回0；否则，返回错误编号
		*/
		//printf("sigwait 之前 %d\n", signo);
		//该函数会阻塞
		err = sigwait(&mask, &signo);
		if (err != 0)
			err_exit(err, "sigwait failed");
		switch (signo) {
		case SIGINT:
			printf("\ninterrupt\n");
			break;

		case SIGQUIT:
			pthread_mutex_lock(&lock);
			quitflag = 1;
			pthread_mutex_unlock(&lock);
			pthread_cond_signal(&waitloc);
			return(0);

		default:
			printf("unexpected signal %d\n", signo);
			exit(1);
		}
	}
}

int main(void)
{
	int			err;
	sigset_t	oldmask;
	pthread_t	tid;

	/*
		头文件：#include <signal.h>
		定义函数：int sigemptyset(sigset_t *set);
		函数说明：sigemptyset()用来将参数set 信号集初始化并清空.
	*/
	sigemptyset(&mask);
	/*
		//表头文件 #include<signal.h>
		//定义函数 int sigaddset(sigset_t *set,int signum);
		//sigaddset()用来将参数signum 代表的信号加入至参数set 信号集里
	*/
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);
	/*
		每个线程均有自己的信号屏蔽集（信号掩码），可以使用pthread_sigmask函数来屏蔽某个线程对某些信号的 响应处理，仅留下需要处理该信号的线程来处理指定的信号。
		实现方式是：利用线程信号屏蔽集的继承关系（在主进程中对sigmask进行设置后，主进程创建出来的线程将继承主进程的掩码）
		//SIG_BLOCK，把信号集添加到线程信号屏蔽字中  结果集是当前集合参数集的并集
		//SIG_SETMASK，用信号集替换线程的信号屏蔽字  结果集是当前集合参数集的差集
		//SIG_UNBLOCK，从线程信号屏蔽字中移除信号集  结果集是由参数集指向的集
		
		如果oset参数不为空，线程之前的信号屏蔽字就存储在它指向的sigset_t结构中。
		线程可以通过把set参数设置为NULL，
		并把oset参数设置为sigset_t结构的地址，来获取当前的信号屏蔽字。这种情况中的how参数会被忽略。
	*/
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0)
	{
		err_exit(err, "SIG_BLOCK error");
	}

	err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0)
	{
		err_exit(err, "can't create thread");
	}

	pthread_mutex_lock(&lock);
	while (quitflag == 0)
	{
		pthread_cond_wait(&waitloc, &lock);
	}
	pthread_mutex_unlock(&lock);

	/* SIGQUIT has been caught and is now blocked; do whatever */
	quitflag = 0;

	/* reset signal mask which unblocks SIGQUIT */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		err_sys("SIG_SETMASK error");
	}
	exit(0);
}
