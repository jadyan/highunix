#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PTHREAD_BARRIER_SIZE 3

pthread_barrier_t barrier;

void err_exit(const char *err_msg)
{
     printf("error:%s\n", err_msg);
     exit(1);
}

void *thread_fun(void *arg)
{
    int result;
    char *thr_name = (char *)arg;
    printf("线程%s工作完成...\n", thr_name);
    result = pthread_barrier_wait(&barrier);
    if (result == PTHREAD_BARRIER_SERIAL_THREAD)
                 printf("线程%s，wait后第一个返回 返回值PTHREAD_BARRIER_SERIAL_THREAD =%d \n", thr_name, PTHREAD_BARRIER_SERIAL_THREAD);
         else if (result == 0)
                 printf("线程%s，wait后返回为0\n", thr_name);
    return NULL;
}

int main(void)
{
    pthread_t tid_1, tid_2, tid_3;

    /* 初始化屏障 */
    pthread_barrier_init(&barrier, NULL, PTHREAD_BARRIER_SIZE+1);

    if (pthread_create(&tid_1, NULL, thread_fun, "1") != 0)
        err_exit("create thread 1");

    if (pthread_create(&tid_2, NULL, thread_fun, "2") != 0)
        err_exit("create thread 2");

    if (pthread_create(&tid_3, NULL, thread_fun, "3") != 0)
        err_exit("create thread 3");

    /* 主线程等待工作完成 */
    pthread_barrier_wait(&barrier);
    printf("所有线程工作已完成...\n");

    sleep(1);
    return 0;
}
