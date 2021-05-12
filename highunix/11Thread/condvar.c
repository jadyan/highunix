#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

/*
 * 条件变量
 * 11.6.6
 * page 332
 * */
struct msg {
	struct msg *m_next;
	char str[128];
};

struct msg *workq=NULL;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;


/*
	传递给pthread_cond_wait的互斥量对条件进行保护。调用者把锁住的互斥量传给函数，
	函数然后自动把调用线程放到等待条件的线程列表上，
	对互斥量解锁。这就关闭了条件检查和线程进入休眠状态等待条件改变这两个操作之间的时间通道，
	这样线程就不会错过条件的任何变化。pthread_cond_wait返回时，互斥量再次被锁住
	
	使用条件变量之前，必须先对它进行初始化。由pthread_cond_t数据类型表示的条件变量可以用两种方式进行初始化，
	可以把常量PTHREAD_COND_INITIALIZER赋给静态分配的条件变量，但是如果条件变量是动态分配的，则需要使用pthread_cond_init函数对它进行初始化。
	在释放条件变量底层的内存空间之前，可以使用pthread_cond_destroy函数对条件变量进行反初始化
	
	 pthread_cond_wait() 用于阻塞当前线程，等待别的线程使用pthread_cond_signal()或pthread_cond_broadcast来唤醒它。
	//pthread_cond_wait() 必须与pthread_mutex 配套使用。pthread_cond_wait()函数一进入wait状态就会自动release mutex。
	//当其他线程通过pthread_cond_signal()或pthread_cond_broadcast，把该线程唤醒，使pthread_cond_wait()通过（返回）时，该线程又自动获得该mutex。
*/
void process_msg(void)
{
	//struct msg *mp;

	for (;;) 
	{
		pthread_mutex_lock(&qlock);
		while (workq == NULL)
		{
			pthread_cond_wait(&qready, &qlock);
		}
		
		struct msg * pworkqtp= workq;
		while( pworkqtp != NULL )
		{
			//读完完10个后 让自己阻塞，等待写的进程把10个写完以后通知
			printf("thread process_msg read %s \n", pworkqtp->str);
			struct msg * ptmpfree = pworkqtp;
			pworkqtp = pworkqtp->m_next;

			free(ptmpfree);
			ptmpfree = NULL;
			sleep (1);
		}
		workq = NULL;
		pthread_cond_broadcast(&qready);
		printf("*****\n");
		//mp = workq;
		//workq = mp->m_next;
		pthread_mutex_unlock(&qlock);
		sleep(1);
		/* now process the message mp */
	}
}


void enqueue_msg(void )
{

	char str[128];

	pthread_mutex_lock(&qlock);
	
	int i;
	while (1)
	{
		memset (str, 0x00, sizeof(str));
		
		sprintf( str,"XXXXXX%dXXXXXX", i++);
		struct msg* pmsg = (struct msg*)malloc(sizeof(struct msg));
		memset(pmsg,0x00, sizeof(struct msg));
		strcpy(pmsg->str, str);
		
		printf("thread enqueue_msg write%s \n", str);
		pmsg->m_next = workq;
		workq = pmsg;
		if((i%10)==0)
		{
			struct msg * pworkqtp= workq;
			pthread_cond_signal(&qready);
			//写完10个后 让自己阻塞，等待读的进程把10个读完以后通知
			pthread_cond_wait(&qready, &qlock);
		}
		pthread_mutex_unlock(&qlock);
		sleep (1);
	}
	


}

#if 0

void process_msg(void)
{
	struct msg *mp;

	for (;;) 
	{
		pthread_mutex_lock(&qlock);
		while (workq == NULL)
		{
			pthread_cond_wait(&qready, &qlock);
		}
		mp = workq;
		workq = mp->m_next;
		pthread_mutex_unlock(&qlock);
		/* now process the message mp */
	}
}

void enqueue_msg(struct msg *mp)
{
	struct msg *mp=NULL;
	pthread_mutex_lock(&qlock);
	mp->m_next = workq;
	workq = mp;
	pthread_mutex_unlock(&qlock);
	pthread_cond_signal(&qready);
}
#endif

int main(int argc ,char **argv)
{
    pthread_t pt_1 = 0;     
    pthread_t pt_2 = 0;     
	int ret = pthread_create(&pt_1, NULL, (void *)process_msg, NULL);
	ret = pthread_create(&pt_2, NULL, (void *)enqueue_msg, NULL	);
	
	while(1)
	{
		sleep(2);
	}
	return 0;
}