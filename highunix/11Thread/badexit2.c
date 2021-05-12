#include "apue.h"
#include <pthread.h>

/*
 * 第11章
 * 11.5 线程创建
 异常退出
 * */
/*
	

	//pthread_exit()函数原型：
	//#include<pthread.h>
	//void pthread_exit(void* retval);
	//参数retval：pthread_exit()调用线程的返回值，可以用pthread_join()函数来检索获取。
	//功能：退出线程。

	
	
	函数pthread_join用来等待一个线程的结束,线程间同步的操作。头文件 ： #include <pthread.h>
	函数定义： int pthread_join(pthread_t thread, void **retval);
	描述 ：pthread_join()函数，以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的。
	参数 ：thread: 线程标识符，即线程ID，标识唯一线程。retval: 用户定义的指针，用来存储被等待线程的返回值。
	返回值 ： 0代表成功。 失败，返回的则是错误号。
*/

struct foo {
	int a, b, c, d;
};

struct foo	foo1;

void
printfoo(const char *s, const struct foo *fp)
{
	printf("%s", s);
	printf("  structure at 0x%lx\n", (unsigned long)fp);
	printf("  foo.a = %d\n", fp->a);
	printf("  foo.b = %d\n", fp->b);
	printf("  foo.c = %d\n", fp->c);
	printf("  foo.d = %d\n", fp->d);
}

void * thr_fn1(void *arg)
{
	//分配在栈上，退出时 栈上的内容就没有了，线程有自己的栈
	//struct foo	foo1 = {1, 2, 3, 4};
	
	//使用全局
	foo1.a = 1;
	foo1.b = 2;
	foo1.c = 3;
	foo1.d = 4;
	
	printfoo("thread 1:\n", &foo1);
	sleep(10);
	pthread_exit((void *)&foo1);
}

void * thr_fn2(void *arg)
{
	printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
	pthread_exit((void *)0);
}

int
main(void)
{
	int			err;
	pthread_t	tid1, tid2;
	struct foo	*fp;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0)
	{
		err_exit(err, "can't create thread 1");
	}
	printf("等待第一个线程 开始\n");
	err = pthread_join(tid1, (void *)&fp);
	printf("等待第一个线程 结束\n");
	if (err != 0)
	{
		err_exit(err, "can't join with thread 1");
	}
	
	printfoo("main thread :\n", fp);
	
	sleep(1);
	printf("parent starting second thread\n");
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
	{
		err_exit(err, "can't create thread 2");
	}
	sleep(1);
	printfoo("parent:\n", fp);
	exit(0);
}
