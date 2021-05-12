#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
 #include <unistd.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

/*
//    11-11
//    page 404
//    产生条件
//        线程对同一个互斥量加锁两次（如果线程试图对同一个互斥量加锁两次，那么它自身就会陷入死锁状态）
//        A和B的锁相互占用。（
            程序中使用一个以上的互斥量时，如果允许一个线程一直占有第一个互斥量，并且在试图锁住第二个互斥量时处于阻塞状态，
            但是拥有第二个互斥量的线程也在试图锁住第一个互斥量。
            因为两个线程都在相互请求另一个线程拥有的资源，所以这两个线程都无法向前运行，于是就产生死锁。）
//    如何避免
//    方法1：多次上锁时让上锁的顺序需要和加锁的顺序相同
//例子没看懂

  假设需要对两个互斥量A和B同时加锁。如果所有线程总是在对互斥量B加锁之前锁住互斥量A，那么使用这两个互斥量就不会产生死锁（当然在其他的资源上仍可能出现死锁）。
  类似地，如果所有的线程总是在锁住互斥量A之前锁住互斥量B，那么也不会发生死锁。可能出现的死锁只会发生在一个线程试图锁住另一个线程以相反的顺序锁住的互斥量
*/
struct foo *fh[NHASH];


/*
    PTHREAD_MUTEX_INITIALIZER 互斥锁
*/
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
    int             f_count;
    pthread_mutex_t f_lock;
    int             f_id;
    struct foo     *f_next; /* protected by hashlock */
    /* ... more stuff here ... */
};

struct foo * foo_alloc(int id) /* allocate the object */
{
    struct foo    *fp;
    int            idx;

    if ((fp = malloc(sizeof(struct foo))) != NULL) 
    {
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);
            return(NULL);
        }
        idx = HASH(id);
        pthread_mutex_lock(&hashlock);
        //把上次的加锁的数据放在这个的next里面， 说明id 是可以重复的
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        /* ... continue initialization ... */
        pthread_mutex_unlock(&fp->f_lock);
    }
    return(fp);
}

void foo_hold(struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}


/*
	foo_find函数锁住散列列表锁，然后搜索被请求的结构。如果找到了，就增加其引用计数并返回指向该结构的指针。
	注意，加锁的顺序是，先在foo_find函数中锁定散列列表锁，然后再在foo_hold函数中锁定foo结构中的f_lock互斥量。
*/
struct foo * foo_find(int id) /* find an existing object */
{
    struct foo    *fp;

    pthread_mutex_lock(&hashlock);     //对第一个锁加锁
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) 
    {
        if (fp->f_id == id) {
            foo_hold(fp);              //如果找到了 就在foo_hold 对第二个锁加锁 修改count++
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return(fp);
}


/*
	foo_rele函数就变得更加复杂了。如果这是最后一个引用，就需要对这个结构互斥量进行解锁，因为我们需要从散列列表中删除这个结构，这样才可以获取散列列表锁，然后重新获取结构互斥量。
	从上一次获得结构互斥量以来我们可能被阻塞着，所以需要重新检查条件，判断是否还需要释放这个结构。
	如果另一个线程在我们为满足锁顺序而阻塞时发现了这个结构并对其引用计数加1，那么只需要简单地对整个引用计数减1，对所有的东西解锁，然后返回。
*/
void foo_rele(struct foo *fp) /* release a reference to the object */
{
    struct foo    *tfp;
    int            idx;

    pthread_mutex_lock(&fp->f_lock);
    if (fp->f_count == 1) { /* last reference */
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_lock(&hashlock);
        pthread_mutex_lock(&fp->f_lock);
        /* need to recheck the condition */
        if (fp->f_count != 1) {
            fp->f_count--;
            pthread_mutex_unlock(&fp->f_lock);
            pthread_mutex_unlock(&hashlock);
            return;
        }
        /* remove from list */
        idx = HASH(fp->f_id);
        tfp = fh[idx];
        if (tfp == fp) {
            fh[idx] = fp->f_next;
        } else {
            while (tfp->f_next != fp)
                tfp = tfp->f_next;
            tfp->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        fp->f_count--;
        pthread_mutex_unlock(&fp->f_lock);
    }
}


void * thr_fn1(void *arg)
{
	printf("thread 1 returning\n");
	
	
	
	while(1)
	{
		int i= 27;
		struct foo* fp  = foo_alloc(27);
		fp  = foo_find(27);
		printf("thread 1\n");
		sleep(1);
	}
	return((void *)1);
}


void * thr_fn2(void *arg)
{
	printf("thread 2 exiting\n");
	
	while(1)
	{
		int i= 27;
		struct foo* fp  = foo_alloc(27);
		fp  = foo_find(27);
		sleep(1);
		printf("thread 2\n");
	}
	
	pthread_exit((void *)2);
}


int main(int argc, char **argv)
{
	//for(int i=0;i<100;i++)
	//{
	//	printf("i=%d HASH(id) = %d \n", i, HASH(i));
	//}
	pthread_t tid , tid2;
	int err=pthread_create(&tid, NULL, thr_fn1, NULL);
	if (err != 0)
	{
		exit(0);
	}
	printf("tid=%d HASH(tid) = %d \n", tid, HASH(tid));
	err =pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
	{
		exit(0);
	}
	printf("tid2=%d HASH(tid2) = %d \n", tid2, HASH(tid2));
	
	
	while(1)
	{
		printf("main timeout\n");
		sleep(1);
	}
}