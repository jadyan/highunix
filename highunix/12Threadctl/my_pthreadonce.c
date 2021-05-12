#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>  

/*
	//���ܣ�������ʹ�ó�ֵΪPTHREAD_ONCE_INIT��once_control������֤init_routine()�����ڱ�����ִ�������н�ִ��һ�Ρ�
	//�ڶ��̱߳�̻����£�����pthread_once()���û�����ڶ���߳��У�init_routine()������ִ��һ�Σ��������ĸ��߳���ִ���ǲ����ģ������ں˵�����������
	//Linux Threadsʹ�û�����������������֤��pthread_once()ָ���ĺ���ִ���ҽ�ִ��һ�Σ���once_control��ʾ�Ƿ�ִ�й���
	//���once_control�ĳ�ֵ����PTHREAD_ONCE_INIT��Linux Threads����Ϊ0����pthread_once() ����Ϊ�ͻ᲻������
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

