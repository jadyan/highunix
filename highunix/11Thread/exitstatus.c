#include "apue.h"
#include <pthread.h>

/*
 * 第11章 11.4 
 * page 312
 * */

/*
	//pthread_exit()函数原型：
	//#include<pthread.h>
	//void pthread_exit(void* retval);
	//参数retval：pthread_exit()调用线程的返回值，可以用pthread_join()函数来检索获取。
	//功能：退出线程。
*/
void * thr_fn1(void *arg)
{
	printf("thread 1 returning\n");
	while(1)
	{
		printf("timeout\n");
		sleep(1);
	}
	return((void *)1);
}

void * thr_fn2(void *arg)
{
	printf("thread 2 exiting\n");
	pthread_exit((void *)2);
}


/*
	//函数定义： int pthread_join(pthread_t thread, void **retval);
	//描述 ：pthread_join()函数，以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的。
	//参数 ：thread: 线程标识符，即线程ID，标识唯一线程。retval: 用户定义的指针，用来存储被等待线程的返回值
*/
int main(void)
{
	int			err;
	pthread_t	tid1, tid2;
	void		*tret;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0)
	{
		err_exit(err, "can't create thread 1");
	}
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
	{
		err_exit(err, "can't create thread 2");
	}
	
	while(1)
	{
		printf("main timeout\n");
		sleep(1);
	}
	
	err = pthread_join(tid1, &tret);
	if (err != 0)
	{
		err_exit(err, "can't join with thread 1");
	}	
	printf("thread 1 exit code %ld\n", (long)tret);
	err = pthread_join(tid2, &tret);
	if (err != 0)
	{
		err_exit(err, "can't join with thread 2");
	}
	printf("thread 2 exit code %ld\n", (long)tret);
	exit(0);
}
