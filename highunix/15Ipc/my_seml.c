#define _XOPEN_SOURCE 700
/*
	https://blog.csdn.net/ljianhui/article/details/10243617
	page 457
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>

/*
 * 执行方式
 * ./my_seml o & ./my_seml
 * */
 
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};
 
static int sem_id = 0;
 
static int set_semvalue();
static void del_semvalue();
static int semaphore_p();
static int semaphore_v();
 
/*
* 	1、semget函数
* 它的作用是创建一个新信号量或取得一个已有信号量，原型为：
* int semget(key_t key, int num_sems, int sem_flags);
* 第一个参数key是整数值（唯一非零），不相关的进程可以通过它访问一个信号量，它代表程序可能要使用的某个资源，程序对所有信号量的访问都是间接的，
* 	程序先通过调用semget函数并提供一个键，再由系统生成一个相应的信号标识符（semget函数的返回值），
    只有semget函数才直接使用信号量键，所有其他的信号量函数使用由semget函数返回的信号量标识符。如果多个程序使用相同的key值，key将负责协调工作。
* 第二个参数num_sems指定需要的信号量数目，它的值几乎总是1。
* 第三个参数sem_flags是一组标志，当想要当信号量不存在时创建一个新的信号量，可以和值IPC_CREAT做按位或操作。设置了IPC_CREAT标志后，即使给出的键是一个已有信号量的键，
  也不会产生错误。而IPC_CREAT | IPC_EXCL则可以创建一个新的，唯一的信号量，如果信号量已存在，返回一个错误。
*/

//用法
//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/sem.h>
//int semop(int semid, struct sembuf *sops, unsigned nsops);
//int semtimedop(int semid, struct sembuf *sops, unsigned nsops, struct timespec *timeout);
//参数
//semid：信号集的识别码，可通过semget获取。
//sops：指向存储信号操作结构的数组指针，信号操作结构的原型如下
//struct sembuf
//{
	//unsigned short sem_num; /* semaphore number */
	//short sem_op; /* semaphore operation */
	//short sem_flg; /* operation flags */
//};
//这三个字段的意义分别为：
//sem_num：操作信号在信号集中的编号，第一个信号的编号是0。
//sem_op：
//sem_op > 0  信号加上 sem_op 的值，表示进程释放控制的资源；(释放锁)
//sem_op = 0  如果没有设置 IPC_NOWAIT，则调用进程进入睡眠状态，直到信号量的值为0；否则进程不回睡眠，直接返回 EAGAIN
//sem_op < 0  信号加上 sem_op 的值。若没有设置 IPC_NOWAIT ，则调用进程阻塞，直到资源可用；否则进程直接返回EAGAIN(加锁)
//sem_flg：信号操作标志，可能的选择有两种
//IPC_NOWAIT //对信号的操作不能满足时，semop()不会阻塞，并立即返回，同时设定错误信息。
//SEM_UNDO //程序结束时(不论正常或不正常)，保证信号值会被重设为semop()调用前的值。这样做的目的在于避免程序在异常情况下结束时未将锁定的资源解锁，造成该资源永远锁定。
//nsops：信号操作结构的数量，恒大于或等于1。
//timeout：当semtimedop()调用致使进程进入睡眠时，睡眠时间不能超过本参数指定的值。如果睡眠超时，semtimedop()将失败返回，并设定错误值为EAGAIN。如果本参数的值为NULL，semtimedop()将永远睡眠等待。
int main(int argc, char *argv[])
{
	char message = 'X';
	int i = 0;
 
	//创建信号量
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
 
	if(argc > 1)
	{
		//程序第一次被调用，初始化信号量
		if(!set_semvalue())
		{
			fprintf(stderr, "Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}
		//设置要输出到屏幕中的信息，即其参数的第一个字符
		message = argv[1][0];
		sleep(2);
	}
	for(i = 0; i < 10; ++i)
	{
		//进入临界区
		if(!semaphore_p())
			exit(EXIT_FAILURE);
		//向屏幕中输出数据
		printf("%c", message);
		//清理缓冲区，然后休眠随机时间
		fflush(stdout);
		sleep(rand() % 3);
		//离开临界区前再一次向屏幕输出数据
		printf("%c", message);
		fflush(stdout);
		//离开临界区，休眠随机时间后继续循环
		if(!semaphore_v())
			exit(EXIT_FAILURE);
		sleep(rand() % 2);
	}
 
	sleep(10);
	printf("\n%d - finished\n", getpid());
 
	if(argc > 1)
	{
		//如果程序是第一次被调用，则在退出前删除信号量
		sleep(3);
		del_semvalue();
	}
	exit(EXIT_SUCCESS);
}
 
static int set_semvalue()
{
	//用于初始化信号量，在使用信号量前必须这样做
	union semun sem_union;
	
//	原型：int semctl(int semid,int semnum,int cmd, /*union semun arg*/);
//	果有第四个参数，它通常是一个union semum结构，定义如下：
//	union semun{
//	    int val;
//	    struct semid_ds *buf;
//	    unsigned short *arry;
//	};
//	前两个参数与前面一个函数中的一样，command通常是下面两个值中的其中一个
//	SETVAL：用来把信号量初始化为一个已知的值。p 这个值通过union semun中的val成员设置，其作用是在信号量第一次使用前对它进行设置。
//	IPC_RMID：用于删除一个已经无需继续使用的信号量标识符

	sem_union.val = 1;
	if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		return 0;
	}
	return 1;
}
 
static void del_semvalue()
{
	//删除信号量
	union semun sem_union;
	if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n");
}
 
static int semaphore_p()
{
	//对信号量做减1操作，即等待P（sv）
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;//P()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return 0;
	}
	return 1;
}
 
static int semaphore_v()
{
	//这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;//V()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_v failed\n");
		return 0;
	}
	return 1;
}
