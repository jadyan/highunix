#define _XOPEN_SOURCE 700
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<semaphore.h>
#include<sys/sem.h>
#include <sys/stat.h>

#include<fcntl.h>

//#include <unistd.h>
//int lockf(int fd, int cmd, off_t len);
//①fd 是打开文件的文件描述符。
//为通过此函数调用建立锁定，文件描述符必须使用只写权限（O_WRONLY）或读写权限（O_RDWR）打开。如果调用进程是具有PRIV_LOCKRDONLY 权限的组的成员，它也可以使用lockf()来锁定使用只读权限（O_RDONLY）打开的文件。
//②cmd 是指定要采取的操作的控制值，允许的值在中定义。
//如下所示：
//# define F_ULOCK 0 //解锁
//# define F_LOCK 1 //互斥锁定区域
//# define F_TLOCK 2 //测试互斥锁定区域
//# define F_TEST 3 //测试区域
//F_ULOCK 请求可以完全或部分释放由进程控制的一个或多个锁定区域。如果区域未完全释放，剩余的区域仍将被进程锁定。如果该表已满，将会返回[EDEADLK]错误，并且不会释放请求的区域。
//使用 F_LOCK 或 F_TLOCK 锁定的区域可以完全或部分包含同一个进程以前锁定的区域，或被同一个进程以前锁定的区域包含。此时，这些区域将会合并为一个区域。如果请求要求将新元素添加到活动锁定表中，但该表已满，则会返回一个错误，并且不会锁定新区域。
//F_LOCK 和 F_TLOCK 请求仅在采取的操作上有所差异（如果资源不可用）。如果区域已被其他进程锁定，F_LOCK 将使调用进程进入休眠状态，直到该资源可用，而 F_TLOCK 则会返回[EACCES]错误。
//F_TEST 用于检测在指定的区域中是否存在其他进程的锁定。如果该区域被锁定，lockf()将返回 -1，否则返回0；在这种情况下，errno 设置为[EACCES]。F_LOCK 和 F_TLOCK 都用于锁定文件的某个区域（如果该区域可用）。F_ULOCK 用于删除文件区域的锁定。
//③len是要锁定或解锁的连续字节数。
//要锁定的资源从文件中当前偏移量开始，对于正 len 将向前扩展，对于负 len 则向后扩展（直到但不包括当前偏移量的前面的字节数）。如果 len 为零，则锁定从当前偏移量到文件结尾的区域（即从当前偏移量到现有或任何将来的文件结束标志）。要锁定一个区域，不需要将该区域分配到文件中，因为这样的锁定可以在文件结束标志之后存在。
//使用 S_ENFMT 文件模式的常规文件（未设置组执行位）将启用强制策略。启用强制策略后，如果清除了 O_NDELAY，访问锁定区域的读取和写入将进入休眠状态，直到整个区域可用为止，但是如果设置了O_NDELAY，将会返回−1并设置 errno。由其他系统函数（如 exec()）访问的文件不受强制策略的影响。
//返回值


/*
	例子说明：管道通信中，如果父进程使用 fork()创建两个子进程1和2，子进程1，2按顺序向管道写一段文字，最后父进程从管道将子进程写入的内容读出来，要保证进程执行的先后顺序，可以用有名信号量来解决。
	https://www.lagou.com/lgeduarticle/77341.html
	page 467
*/

#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<semaphore.h>
#include<sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include<fcntl.h>

int main(){
    int pid1,pid2;
    int Cpid1,Cpid2=-1;
    int fd[2];//0为读出段,1为写入端
    char outpipe1[100],inpipe[200],outpipe2[100];
    void *shm = NULL;
    sem_t *shared;
    int shmid = shmget((key_t)(1234), sizeof(sem_t *), 0666 | IPC_CREAT);//创建一个共享内存，返回一个标识符
    if(shmid == -1){
        perror("shmat :");
        exit(0);
    }
    shm = shmat(shmid, 0, 0);//返回指向共享内存第一个字节的指针
    shared = (sem_t *)shm;
    sem_init(shared, 1, 0);//初始化共享内存信号量值为0 ，第二个参数 1 说明多个进程共享
    pipe(fd);//建立一个无名管道

    pid1 = fork();
    if(pid1<0){
        printf("error in the first fork!");
    }else if(pid1==0){//子进程1

        Cpid1 = getpid();
        close(fd[0]);//关掉读出端
        lockf(fd[1],1,0);//上锁,则锁定从当前偏移量到文件结尾的区域
        sprintf(outpipe1,"Child process 1 is sending a message!\n");
        write(fd[1],outpipe1,strlen(outpipe1));
        lockf(fd[1],0,0);//解锁
        sem_post(shared); //+1

        exit(0);
   }else{

        pid2 = fork();
        if(pid2<0){
            printf("error in the second fork!\n");
        }else if(pid2==0){
                sem_wait(shared);  //-1
                Cpid2 = getpid();
				close(fd[0]);
                lockf(fd[1],1,0);
                sprintf(outpipe2,"Child process 2 is sending a message!\n");

                write(fd[1],outpipe2,strlen(outpipe2));
                lockf(fd[1],0,0);//解锁

                exit(0);
        }
        if(pid1 > 0 && pid2 >0){

                waitpid(pid2,NULL,0);//同步,保证子进程先写父进程再读
                close(fd[1]);//关掉写端
                read(fd[0],inpipe,200);
                printf("%s\n",inpipe);

                exit(0);
        }

    }
    return 0;
}

