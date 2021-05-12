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
       �����ļ������keypָ����ڴ浥Ԫ��������ɱ������е������߳�ʹ�ã�
       ��ÿ���̰߳�������벻ͬ���߳�˽�����ݵ�ַ���й�����
       ���̵߳���pthread_exit�򷵻أ������˳�ʱ�������destructor��
       ������̵߳���exit��_exit��_Exit��abort�������������˳�ʱ���Ͳ����������������
       �߳̿���Ϊ�߳�˽�����ݷ���������ÿ������������һ������������������
       
       �߳̿���Ϊ�߳��ض����ݷ���������ÿ������������һ��������������������
       ÿ�����������������Ի�����ͬ����Ȼ���м�Ҳ����ʹ����ͬ������������ÿ������ϵͳʵ�ֿ��ԶԽ��̿ɷ���ļ���������������
    */
	pthread_key_create(&key, free);
}

/*
	��һ�������Ժ󣬾Ϳ���ͨ������pthread_setspecific�����Ѽ����߳��ض����ݹ�������������ͨ��pthread_getspecific��������߳��ض����ݵĵ�ַ��

	//#include <pthread.h>
	//void *pthread_getspecific(pthread_key_t key);
	//����ֵ���߳��ض�����ֵ����û��ֵ��ü�����������NULL
	//int pthread_setspecific(pthread_key_t key, const void* value);
	//����ֵ�����ɹ�������0�����򣬷��ش�����
*/

char * getenv(const char *name)
{
	int		i, len;
	char	*envbuf;

	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&env_mutex);
    /*
       void *pthread_getspecific(pthread_key_t key);
       ����߳�˽�����ݵĵ�ַ
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
           �Ѽ����߳�˽�����ݹ�������
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

