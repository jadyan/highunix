#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>  

/*
	//功能：本函数使用初值为PTHREAD_ONCE_INIT的once_control变量保证init_routine()函数在本进程执行序列中仅执行一次。
	//在多线程编程环境下，尽管pthread_once()调用会出现在多个线程中，init_routine()函数仅执行一次，究竟在哪个线程中执行是不定的，是由内核调度来决定。
	//Linux Threads使用互斥锁和条件变量保证由pthread_once()指定的函数执行且仅执行一次，而once_control表示是否执行过。
	//如果once_control的初值不是PTHREAD_ONCE_INIT（Linux Threads定义为0），pthread_once() 的行为就会不正常。
*/
  
pthread_once_t once = PTHREAD_ONCE_INIT;  
  
void once_run(void)  
{  
        //cout<<"once_run in thread "<<(unsigned int )pthread_self()<<endl;
        printf("once_run in thread=%u\n", pthread_self());
}  
  
void * child1(void * arg)  
{  
        pthread_t tid =pthread_self();  
        //cout<<"thread "<<(unsigned int )tid<<" enter"<<endl;
        printf("thread1 %u enter\n", tid);
        pthread_once(&once,once_run);  
        //cout<<"thread "<<tid<<" return"<<endl;
        printf("thread2 %u return\n", tid);
}  
  
  
void * child2(void * arg)  
{  
        pthread_t tid =pthread_self();  
        //cout<<"thread "<<(unsigned int )tid<<" enter"<<endl;
        printf("thread2 %u enter\n", tid);
        pthread_once(&once,once_run);  
        //cout<<"thread "<<tid<<" return"<<endl;
        printf("thread2 %u return\n", tid);
}  
  
int main(void)  
{  
        pthread_t tid1,tid2;  
        //cout<<"hello"<<endl;
        printf("start pid tid %d ,%d \n",getpid(), pthread_self());
        pthread_once(&once,once_run);
        pthread_create(&tid1,NULL,child1,NULL);  
        pthread_create(&tid2,NULL,child2,NULL);  
        sleep(10);  
        //cout<<"main thread exit"<<endl;  
        printf("main thread exit\n");
        return 0;  
}

