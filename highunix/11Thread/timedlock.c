#include "apue.h"
#include <pthread.h>

/*
 * #include <pthread.h>
 * #include <time.h>
 * int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex,const struct timespec *restrict tsptr);
 *返回值：若成功，返回0；否则，返回错误编号


#include<time.h>
//int clock_gettime(clockid_t clk_id,struct timespec *tp);

//函数"clock_gettime"是基于Linux C语言的时间函数,他可以用于计算精度和纳秒
//clk_id : 检索和设置的clk_id指定的时钟时间。
//CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,
//中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
//　　CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
//　　CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间
//　　CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间

 * */
int
main(void)
{
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	/*
		在使用互斥变量以前，必须首先对它进行初始化，可以把它设置为常量PTHREAD_MUTEX_INITIALIZER（只适用于静态分配的互斥量），
		也可以通过调用pthread_mutex_init函数进行初始化。如果动态分配互斥量（例如，通过调用malloc函数），在释放内存前需要调用pthread_mutex_destroy。
	*/
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutex is locked\n");
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	/*
		%r 12小时的时间
	*/
	strftime(buf, sizeof(buf), "%r", tmp);
	//printf("current time is %s\n", buf);
	printf("current time is %s, tout.tv_sec =%d \n", buf, tout.tv_sec);
	tout.tv_sec += 10;	/* 10 seconds from now */
	/* caution: this could lead to deadlock */
	/*
		超时指定愿意等待的绝对时间(与相对时间对比而言，指定在时间X之前可以阻塞等待，而不是说愿意阻塞Y秒)。
		这个超时时间是用timespec结构来表示，它用秒和纳秒来描述时间。
	*/
	err = pthread_mutex_timedlock(&lock, &tout);
	
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	//printf("the time is now %s\n", buf);
	printf("the time is now %s tout.tv_sec =%d \n", buf, tout.tv_sec);
	if (err == 0)
		printf("mutex locked again!\n");
	else
		printf("can't lock mutex again: %s\n", strerror(err));
	exit(0);
}
