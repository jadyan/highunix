#include "apue.h"
#include <pthread.h>

/*
 * 第11章  11.3
 * page 312
 * */
void
cleanup(void *arg)
{
	printf("cleanup: %s\n", (char *)arg);
}


/*
	//pthread_cleanup_push来注册清理函数rtn,这个函数有一个参数arg。在以下三种情形之一发生时，注册的清理函数被执行：
    //1）调用pthread_exit。（同时线程不论是否以非0参数调用pthread_cleanup_pop）
    //2）作为对取消线程请求(pthread_cancel)的响应。
    //3）以非0参数调用pthread_cleanup_pop，（同时线程不能是以return 方式结束的, ）
               当pthread_cleanup_pop()函数的参数为0时，仅仅在线程调用pthread_exit函数或者其它线程对本线程调用 , pthread_cancel函数时，才在弹出“清理函数”的同时执行该“清理函数”。
*/

/*
	3 .pthread_cleanup_push()函数与pthread_cleanup_pop()函数必须成对的出现在同一个函数中。
	4.在线程宿主函数中主动调用return，如果return语句包含在pthread_cleanup_push()/pthread_cleanup_pop()对中，则不会引起清理函数的执行，反而会导致segment fault。	
*/
void * thr_fn1(void *arg)
{
	printf("thread 1 start\n");
	pthread_cleanup_push(cleanup, "thread 1 first handler");
	pthread_cleanup_push(cleanup, "thread 1 second handler");
	printf("thread 1 push complete\n");
	if (arg)
	{
		return((void *)1);
		//pthread_exit((void *)1);
	}
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	//不使用pthread_exit 退出 不会调用pthread_cleanup_push  
	//return((void *)1);  
	//pthread_exit((void *)1);
}

void * thr_fn2(void *arg)
{
	printf("thread 2 start\n");
	
	//一个线程可以注册多个清理注册函数，处理程序记录在栈中，也就说  它们的执行顺序与注册是相反的
	//先调用thread 2 second handler ，再调用thread 2 first handler
	pthread_cleanup_push(cleanup, "thread 2 first handler");
	pthread_cleanup_push(cleanup, "thread 2 second handler");
	printf("thread 2 push complete\n");
	if (arg)
	{
		//当前异常终止时候，pthread_cleanup_push 不会被调用， 在此pthread_cleanup_pop 会报错，不能再if分支中
		//pthread_cleanup_pop(0);
		//pthread_cleanup_pop(0);
		//函数在 pthread_cleanup_push 与 pthread_cleanup_pop之间返回时，栈已被改写， 会产生未定义行为，最好用pthread_exit
		pthread_exit((void *)2);
	}
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void *)2);
}

int main(void)
{
	int			err;
	pthread_t	tid1, tid2;
	void		*tret;

	err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
	if (err != 0)
	{
		err_exit(err, "can't create thread 1");
	}
	err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
	//err = pthread_create(&tid2, NULL, thr_fn2, (void *)NULL);
	if (err != 0)
	{
		err_exit(err, "can't create thread 2");
	}

	err = pthread_join(tid1, &tret);
	if (err != 0)
	{
		err_exit(err, "can't join with thread 1");
	}

	printf("thread 1 exit code %ld\n", (long)tret);
	err = pthread_join(tid2, &tret);
	if (err != 0)
	{
		err_exit(err, "can't join with thread 2");
	}
	printf("thread 2 exit code %ld\n", (long)tret);
	exit(0);
}
