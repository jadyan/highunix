#include "apue.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
/*
 * page 343 线程属性
 * */
int makethread(void *(*fn)(void *), void *arg)
{
	int				err;
	pthread_t		tid;
	pthread_attr_t	attr;

	err = pthread_attr_init(&attr);
	if (err != 0)
	{
		return(err);
	}
	/*
		PTHREAD_CREATE_DETACHED，以分离状态启动线程
		PTHREAD_CREATE_JOINABLE,正常启动线程，应用程序可以获取线程的终止状态
		可以调用pthread_attr_getdetachstate函数获取当前的detachstate线程属性。
		第二个参数所指向的整数要么设置成PTHREAD_CREATE_DETACHED，要么设置成 PTHREAD_CREATE_JOINABLE，具体要取决于给定pthread_attr_t结构中的属性值。
	*/
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err == 0)
	{
		err = pthread_create(&tid, &attr, fn, arg);
	}
	pthread_attr_destroy(&attr);
	return(err);
}

void* run(void * arg)
{
	while(1)
	{
		printf("thread1: %s\n", arg);
		sleep(2);
	}
}
int main(int argc, char **argv)
{
	pthread_t tid;
	char str[]="[thread start]";
	makethread(run,(void*)str);
	while(1)
	{
		printf("main thread\n");
		sleep(2);
	}
}
