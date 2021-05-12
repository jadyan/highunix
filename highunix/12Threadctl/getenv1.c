#define _XOPEN_SOURCE 700
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


/*
 * 重入
 * 12.5
 * page357
 * */
#define MAXSTRINGSZ	4096

static char envbuf[MAXSTRINGSZ];

extern char **environ;

pthread_barrier_t barrier;

char * getenv(const char *name)
{
	int i, len;

	len = strlen(name);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) &&
		  (environ[i][len] == '=')) {
			strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
			return(envbuf);
		}
	}
	return(NULL);
}

void * run1(void* argc)
{
	while(1)
	{
		pthread_barrier_wait(&barrier);
		char *str = getenv("LANG");
		printf("thread thread %s\n",str);
		sleep(1);
	}
}

void * run2(void* argc)
{
	while(1)
	{
		pthread_barrier_wait(&barrier);
		char *str = getenv("LANG");
		printf("thread thread %s\n",str);
		
		sleep(1);
	}
}
int main(int argc ,char **argv)
{
	pthread_t tid1,tid2;
	pthread_barrier_init(&barrier, NULL, 2+1);
	pthread_create(&tid1,NULL,run1,NULL);
	pthread_create(&tid2,NULL,run2,NULL);
	while(1)
	{
		pthread_barrier_wait(&barrier);
		char * str = getenv("LANG");
		printf("main thread %s\n",str);
		sleep(1);
	}
	return 0;
}
