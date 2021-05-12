#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>


/*
 *重入 线程安全的
  page 357·
  barrier 与mutex 不能一起用
 * */
extern char **environ;

pthread_mutex_t env_mutex;

pthread_barrier_t barrier;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void)
{
	pthread_mutexattr_t attr;

	//pthread_barrier_init(&barrier, NULL, 2+1);
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

int getenv_r(const char *name, char *buf, int buflen)
{
	int i, len, olen;

	pthread_once(&init_done, thread_init);
	len = strlen(name);
	pthread_mutex_lock(&env_mutex);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) &&
		  (environ[i][len] == '=')) {
			olen = strlen(&environ[i][len+1]);
			if (olen >= buflen) {
				pthread_mutex_unlock(&env_mutex);
				return(ENOSPC);
			}
			strcpy(buf, &environ[i][len+1]);
			pthread_mutex_unlock(&env_mutex);
			return(0);
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return(ENOENT);
}

void * run1(void* argc)
{
	while(1)
	{
		printf("thread1 start\n");
		//pthread_barrier_wait(&barrier);
		//printf("thread1 barrier\n");
		char str[1024];
		memset(str,0x00,1024);
		getenv_r("LANG",str,1024);
		printf("thread thread %s\n",str);
		sleep(1);
	}
}

void * run2(void* argc)
{
	while(1)
	{
		printf("thread2 start\n");
		//pthread_barrier_wait(&barrier);
		//printf("thread2 barrier\n");
		char str[1024];
		memset(str,0x00,1024);
		getenv_r("LANG",str,1024);
		printf("thread thread %s\n",str);
		
		sleep(1);
	}
}

int main(int argc ,char **argv)
{
	printf("main start\n");
	pthread_t tid1,tid2;
	//pthread_barrier_init(&barrier, NULL, 2+1);
	pthread_create(&tid1,NULL,run1,NULL);
	pthread_create(&tid2,NULL,run2,NULL);
	while(1)
	{
		//pthread_barrier_wait(&barrier);
		//printf("main barrier\n");
		char str[1024];
		memset(str,0x00,1024);
		getenv_r("LANG",str,1024);
		printf("main thread %s\n",str);
		sleep(1);
	}
	return 0;
}
