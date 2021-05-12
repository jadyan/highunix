#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>



//struct shmid_ds{
//      struct ipc_perm shm_perm;/* 操作权限*/
//       int shm_segsz;                    /*段的大小（以字节为单位）*/
//      time_t shm_atime;          /*最后一个进程附加到该段的时间*/
//       time_t shm_dtime;          /*最后一个进程离开该段的时间*/
//      time_t shm_ctime;          /*最后一个进程修改该段的时间*/
//      unsigned short shm_cpid;   /*创建该段进程的pid*/
//       unsigned short shm_lpid;   /*在该段上操作的最后1个进程的pid*/
//       short shm_nattch;          /*当前附加到该段的进程的个数*/
///*下面是私有的*/
//        unsigned short shm_npages;  /*段的大小（以页为单位）*/
//      unsigned long *shm_pages;   /*指向frames->SHMMAX的指针数组*/
//      struct vm_area_struct *attaches; /*对共享段的描述*/
//};


//shmget()函数
//该函数用来创建共享内存，它的原型为：
//int shmget(key_t key, size_t size, int shmflg);
//第一个参数，与信号量的semget函数一样，程序需要提供一个参数key（非0整数），它有效地为共享内存段命名，shmget()函数成功时返回一个与key相关的共享内存标识符（非负整数），
			  //用于后续的共享内存函数。调用失败返回-1.
//不相关的进程可以通过该函数的返回值访问同一共享内存，它代表程序可能要使用的某个资源，程序对所有共享内存的访问都是间接的，程序先通过调用shmget()函数并提供一个键，
//再由系统生成一个相应的共享内存标识符（shmget()函数的返回值），只有shmget()函数才直接使用信号量键，所有其他的信号量函数使用由semget函数返回的信号量标识符。
//第二个参数，size以字节为单位指定需要共享的内存容量
//第三个参数，shmflg是权限标志，它的作用与open函数的mode参数一样，如果要想在key标识的共享内存不存在时，创建它的话，可以与IPC_CREAT做或操作。
//              共享内存的权限标志与文件的读写权限一样，举例来说，0644,它表示允许一个进程创建的共享内存被内存创建者所拥有的进程向共享内存读取和写入数据，同时其他用户创建的进程只能读取共享内存。


#define TEXT_SZ 4096
 
struct shared_use_st
{
	int written;//作为一个标志，非0：1表示可读，0表示可写
	char text[TEXT_SZ];//记录写入和读取的文本
};

int main()
{
	int running = 1;//程序是否继续运行的标志
	void *shm = NULL;//分配的共享内存的原始首地址
	struct shared_use_st *shared;//指向shm
	int shmid;//共享内存标识符
	//创建共享内存
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
	if(shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	
//void *shmat(int shmid, const void *shmaddr, int shmflg)
//函数传入值
//shmid	
//共享内存标识符
//shmaddr
//指定共享内存出现在进程内存地址的什么位置，直接指定为NULL让内核自己决定一个合适的地址位置
//shmflg
//SHM_RDONLY：为只读模式，其他为读写模式
	//将共享内存连接到当前进程的地址空间
	shm = shmat(shmid, 0, 0);
	if(shm == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	printf("\nMemory attached at %X\n", (long)shm);
	//设置共享内存
	shared = (struct shared_use_st*)shm;
	shared->written = 0;
	while(running)//读取共享内存中的数据
	{
		//没有进程向共享内存定数据有数据可读取
		if(shared->written != 0)
		{
			printf("You wrote: %s", shared->text);
			sleep(rand() % 3);
			//读取完数据，设置written使共享内存段可写
			shared->written = 0;
			//输入了end，退出循环（程序）
			if(strncmp(shared->text, "end", 3) == 0)
				running = 0;
		}
		else
		{//有其他进程在写数据，不能读取数据
			sleep(1);
		}
	}
	//把共享内存从当前进程中分离
	if(shmdt(shm) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	//删除共享内存
	if(shmctl(shmid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
