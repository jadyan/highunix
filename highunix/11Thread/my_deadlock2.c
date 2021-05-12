#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *死锁test
 线程1 锁住g_lock1 期待对g_lock2 上锁
 线程2 锁住g_lock2 期待对g_lock1 上锁
 * */

pthread_mutex_t g_lock1;
pthread_mutex_t g_lock2;

void * thr_fn1(void *arg)
{
	while (1)
	{
		printf("thr1 start\n");
		pthread_mutex_lock(&g_lock1);
		pthread_mutex_lock(&g_lock2);
		pthread_mutex_unlock(&g_lock1);
		pthread_mutex_unlock(&g_lock2);
		printf("thr1 end\n");
		sleep(1);

	}
	pthread_exit((void *)0);
}

void * thr_fn2(void *arg)
{
	while (1)
	{
		printf("thr2 start\n");
		pthread_mutex_lock(&g_lock2);
		sleep(1);
		pthread_mutex_lock(&g_lock1);
		pthread_mutex_unlock(&g_lock2);
		pthread_mutex_unlock(&g_lock1);
		printf("thr2 end\n");
		sleep(1);
	}
	pthread_exit((void *)0);
}


int main(int argc, char **argv)
{
	pthread_t tid1, tid2;
	pthread_mutex_init(&g_lock1, NULL);
	pthread_mutex_init(&g_lock2, NULL);
	int err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	while(1)
	{
		printf("main\n");
		sleep(1);
	}
	pthread_mutex_destroy(&g_lock1);
	pthread_mutex_destroy(&g_lock2);
	return 0;
}
