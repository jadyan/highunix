#include "apue.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

/*
//	面的代码有虽然比较接近实际应用，但是对于阐述知识点来说有些冗余（感兴趣可以都看看），或者只看红字部分就足够了。
//
//运行结果如下：
//
//　　在当前路径下生成了retry.dat的文件，OK。
//
//上面的结果很好分析：
//
//　　（1）main中对mutex上锁，调用timeout并在其中由makethread产生了一个新的线程timeout_helper（延迟10秒调用retry函数），main对mutex解锁
//
//　　（2）线程timeout_helper延迟10秒后调用retry函数（此时mutex早已在main中被解锁），对mutex上锁，执行操作，再对mutex解锁。
//
//但如果将main函数中的两条语句调换顺序：
//
//sleep(15);    
//pthread_mutex_unlock(&mutex);
//结果就不同了，这次不会输出retry.dat文件。
//
//因为main线程先等待15秒，而这15秒一直占着mutex；同时，timeout_helper被mutex阻塞了，直到main线程执行unlock操作。
//
//但是main一旦unlock，主进程就exit了；由于timeout_helper也是由这个进程产生了，因此没来得及执行呢就被一起收尸了。
//
//这里容易产生个误区：makethread中有这么一句话：
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
    	递归锁
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