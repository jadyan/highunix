#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXSTRINGSZ	4096

static pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ;

static void thread_init(void)
{
    /* 
       int pthread_key_create(pthread_key_t *keyp, void (*destructor)(void *)); 
       创建的键存放在keyp指向的内存单元，这个键可被进程中的所有线程使用，
       但每个线程把这个键与不同的线程私有数据地址进行关联。
       当线程调用pthread_exit或返回，正常退出时，会调用destructor，
       但如果线程调用exit、_exit、_Exit、abort或其他非正常退出时，就不会调用析构函数。
       线程可以为线程私有数据分配多个键，每个键都可以由一个析构函数与它关联
       
       线程可以为线程特定数据分配多个键，每个键都可以有一个析构函数与它关联。
       每个键的析构函数可以互不相同，当然所有键也可以使用相同的析构函数。每个操作系统实现可以对进程可分配的键的数量进行限制
    */
	pthread_key_create(&key, free);
}

/*
	键一旦创建以后，就可以通过调用pthread_setspecific函数把键和线程特定数据关联起来。可以通过pthread_getspecific函数获得线程特定数据的地址。

	//#include <pthread.h>
	//void *pthread_getspecific(pthread_key_t key);
	//返回值：线程特定数据值；若没有值与该键关联，返回NULL
	//int pthread_setspecific(pthread_key_t key, const void* value);
	//返回值：若成功，返回0；否则，返回错误编号
*/

char * getenv(const char *name)
{
	int		i, len;
	char	*envbuf;

	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&env_mutex);
    /*
       void *pthread_getspecific(pthread_key_t key);
       获得线程私有数据的地址
    */
	envbuf = (char *)pthread_getspecific(key);
	if (envbuf == NULL) {
		envbuf = malloc(MAXSTRINGSZ);
		if (envbuf == NULL) {
			pthread_mutex_unlock(&env_mutex);
			return(NULL);
		}
       /*
           int pthread_setspecific(pthread_key_t key, const void *value);
           把键和线程私有数据关联起来
        */
		pthread_setspecific(key, envbuf);
	}
	len = strlen(name);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) &&
		  (environ[i][len] == '=')) {
			strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
			pthread_mutex_unlock(&env_mutex);
			return(envbuf);
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return(NULL);
}


void * run1(void* argc)
{
	while(1)
	{
		//pthread_barrier_wait(&barrier);
		char *str = getenv("LANG");
		printf("thread thread %s\n",str);
		sleep(1);
	}
}

void * run2(void* argc)
{
	while(1)
	{
		//pthread_barrier_wait(&barrier);
		char *str = getenv("LANG");
		printf("thread thread %s\n",str);
		
		sleep(1);
	}
}
int main(int argc ,char **argv)
{
	pthread_t tid1,tid2;
	//pthread_barrier_init(&barrier, NULL, 2+1);
	pthread_create(&tid1,NULL,run1,NULL);
	pthread_create(&tid2,NULL,run2,NULL);
	while(1)
	{
		//pthread_barrier_wait(&barrier);
		char * str = getenv("LANG");
		printf("main thread %s\n",str);
		sleep(1);
	}
	return 0;
}

