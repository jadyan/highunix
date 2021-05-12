//#define _XOPEN_SOURCE 500

/*
    _GNU_SOURCE它告诉编译器包含X / Open和POSIX标准中定义的一些额外函数的定义。
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//#include "wrlock.h"



/*
 * 读写锁
 作业请求队列由单个读写锁保护。这个例子给出了图11-1所示的一种可能的实现，多个工作线程获取单个主线程分配给它们的作业。
 * */

struct job 
{
    struct job *j_next;
    struct job *j_prev;
    pthread_t   j_id;   /* tells which thread handles this job */
    void *( *job_func)( void *);
};

struct queue 
{
    struct job      *q_head;
    struct job      *q_tail;
    pthread_rwlock_t q_lock;
};

/*
 * Initialize a queue.
 */
int queue_init(struct queue *qp)
{
    int err;

    qp->q_head = NULL;
    qp->q_tail = NULL;
    //初始化读写锁
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0)
    {
        return(err);
    }
    /* ... continue initialization ... */
    return(0);
}

/*
 *从头插入 双向链表
 * Insert a job at the head of the queue.
 */
void job_insert(struct queue *qp, struct job *jp)
{
    //写锁
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL)
    {
        qp->q_head->j_prev = jp;
    }
    else
    {
        qp->q_tail = jp;    /* list was empty */
    }
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 *从尾插入
 * Append a job on the tail of the queue.
 */
void job_append(struct queue *qp, struct job *jp)
{
	//写锁
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if (qp->q_tail != NULL)
    {
        qp->q_tail->j_next = jp;
    }
    else
    {
        qp->q_head = jp;    /* list was empty */
    }
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Remove the given job from a queue.
 */
void job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp)
        {
            qp->q_tail = NULL;
        }
        else
        {
            jp->j_next->j_prev = jp->j_prev;
        }
    } else if (jp == qp->q_tail) 
    {
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Find a job for the given thread ID.
 */
struct job * job_find(struct queue *qp, pthread_t id)
{
    struct job *jp;

    //读锁
    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
    {
        return(NULL);
    }

    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
    {
        if (pthread_equal(jp->j_id, id))
        {
            break;
        }
    }

    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}


void * task1( void *str1 )
{
    printf("I am thread 1, %s\n",(char *)str1);
}

void * task2( void *str2 )
{
    printf("I am thread 2, %s\n",(char *)str2);
}

void * task3( void *str3 )
{
    printf("I am thread 3, %s\n",(char *)str3);
}

void * thr1_func( void *arg ) //arg points to queue.
{
    pthread_t   tid;
    struct job  *jp1;

    tid = pthread_self();
    while (1)
    {
        if ((jp1 = job_find(arg,tid)) != NULL)
        {
            jp1->job_func("task completly!");
            job_remove(arg,jp1); 
            free(jp1);
        }else{
            //线程让出CPU
            pthread_yield();
        }
    }
}

void * thr2_func( void *arg ) //arg points to queue.
{
    pthread_t   tid;
    struct job  *jp2;

    tid = pthread_self();
    while (1){
        if ((jp2 = job_find(arg,tid)) != NULL){
            jp2->job_func("task completly!");
            job_remove(arg,jp2); 
            free(jp2);
        }else{
            pthread_yield();
        }
    }
}

void *thr3_func( void *arg ) //arg points to queue.
{
    pthread_t   tid;
    struct job  *jp3;

    tid = pthread_self();
    while (1){
        if ((jp3 = job_find(arg,tid)) != NULL){
            jp3->job_func("task completly!");
            job_remove(arg,jp3); 
            free(jp3);
        }else{
            pthread_yield();
        }
    }
}

int  main( void )
{
    unsigned long   n;
    int         err;
    int         foo;
    struct job  *jobptr;
    struct queue    *qptr;
    pthread_t   tid[3];
    void *(*func[3])(void *) = { task1, task2, task3 };

    /* initialise the queue */
    if ((qptr = malloc(sizeof( struct queue ))) == NULL)
    {
        exit(-1);
    }
    queue_init(qptr);

    /* produce task in main thread */
    err = pthread_create(&tid[0],NULL,thr1_func,qptr);
    if (err != 0)
    {
        exit(-1);
    }
    else
    {
        printf("create thread 1 success\n");
    }
    err = pthread_create(&tid[1],NULL,thr2_func,qptr);
    if (err != 0)
    {
        exit(-1);
    }
    else
    {
        printf("create thread 2 success\n");
    }
    err = pthread_create(&tid[2],NULL,thr3_func,qptr);
    if (err != 0)
    {
        exit(-1);
    }
    else
    {
        printf("create thread 3 success\n");
    }
    
    for ( n = 1; n <= 10; n++)
    {
        if ((jobptr = malloc(sizeof(struct job))) == NULL)
        {
            exit(-1);
        }
        foo = rand()%3;
        jobptr->j_id = tid[foo];
        jobptr->job_func = func[foo];
        job_insert(qptr,jobptr);
    }

    sleep(10);
    exit(0);
}
