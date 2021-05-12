#include "apue.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

/*
//	��Ĵ�������Ȼ�ȽϽӽ�ʵ��Ӧ�ã����Ƕ��ڲ���֪ʶ����˵��Щ���ࣨ����Ȥ���Զ�������������ֻ�����ֲ��־��㹻�ˡ�
//
//���н�����£�
//
//�����ڵ�ǰ·����������retry.dat���ļ���OK��
//
//����Ľ���ܺ÷�����
//
//������1��main�ж�mutex����������timeout����������makethread������һ���µ��߳�timeout_helper���ӳ�10�����retry��������main��mutex����
//
//������2���߳�timeout_helper�ӳ�10������retry��������ʱmutex������main�б�����������mutex������ִ�в������ٶ�mutex������
//
//�������main�����е�����������˳��
//
//sleep(15);    
//pthread_mutex_unlock(&mutex);
//����Ͳ�ͬ�ˣ���β������retry.dat�ļ���
//
//��Ϊmain�߳��ȵȴ�15�룬����15��һֱռ��mutex��ͬʱ��timeout_helper��mutex�����ˣ�ֱ��main�߳�ִ��unlock������
//
//����mainһ��unlock�������̾�exit�ˣ�����timeout_helperҲ����������̲����ˣ����û���ü�ִ���ؾͱ�һ����ʬ�ˡ�
//
//�������ײ�����������makethread������ôһ�仰��
*/

int makethread(void *(*fn)(void *), void *arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    if ( err!=0 )
        return err;
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if ( err==0 )
        err = pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destroy(&attr);
    return err;
}

struct to_info{
    void (*to_fn)(void *);
    void *to_arg;
    struct timespec to_wait;
};

#define SECTONSEC 1000000000
#define USECTONSEC 1000

void * timeout_helper(void *arg)
{
    struct to_info *tip;
    tip = (struct to_info *)arg;
    clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);
    (*tip->to_fn)(tip->to_arg);
    free(arg);
    return 0;
}

void timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
    struct timespec now;
    struct to_info *tip;
    int err;

    clock_gettime(CLOCK_REALTIME, &now);

    if ( (when->tv_sec > now.tv_sec) || (when->tv_sec==now.tv_sec && when->tv_nsec > now.tv_nsec)  )
    {
        tip = malloc(sizeof(struct to_info));
        if ( tip!=NULL )
        {
            tip->to_fn = func;
            tip->to_arg = arg;
            tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
            if ( when->tv_nsec>=now.tv_nsec )
            {
                tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
            }
            else
            {
                tip->to_wait.tv_sec--;
                tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;
            }
            err = makethread(timeout_helper, (void *)tip);
            if (err==0)
            {
                printf("makethread return 0\n");
                return;
            }
            else
                free(tip);
        }
    }

    (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void retry(void *arg)
{
    printf("require mutex for the second time\n");
    pthread_mutex_lock(&mutex);
    // perform retry
    FILE *fp;
    fp = fopen("./retry.dat","w+");
    fputs("this a retry.",fp);
    fclose(fp);
    pthread_mutex_unlock(&mutex);
}

int main()
{
    int err, condition, arg;
    struct timespec when;

    if ( (err = pthread_mutexattr_init(&attr)) != 0 )
        err_exit(err, "pthread_mutexattr_init failed");
    /*
    	�ݹ���
    */
    if ( (err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) != 0 )
        err_exit(err, "can't set recursive type");
    if ( (err = pthread_mutex_init(&mutex, &attr)) != 0 )
        err_exit(err, "can't create recursive mutex");

    condition = 1;
    pthread_mutex_lock(&mutex);

    if (condition)
    {

        printf("retry until timeout\n");
        clock_gettime(CLOCK_REALTIME, &when);
        when.tv_sec += 10;
        timeout(&when, retry, (void *)((unsigned long)arg));
    }

    pthread_mutex_unlock(&mutex);
    sleep(15);
    exit(0);
}