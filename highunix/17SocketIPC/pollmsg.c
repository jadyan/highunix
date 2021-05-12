#include "apue.h"
#include <poll.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>


/*
	https://blog.csdn.net/zhuxiaoping54532/article/details/51701549
	https://blog.csdn.net/zhouzhenhe2008/article/details/75807720
	https://blog.csdn.net/lianghe_work/article/details/46534029
	我们先来看看socketpair函数的原型如下：
*      int socketpair（int domain，int type，int protocol，int sv[]）
*      第一个参数表示协议族，必须为AF_LOCAL;
*      第二个参数表示类型，既可以是SOCK_STREAM,又可以是SOCK_DGRAM，当参数指定为SOCK_STREAM时，得到的结果称为流管道，
*               它与一般管道的区别是留管道是全双工的，即两个描述符即可读有可写;
*      第三个参数只能为0；
*      第四个参数用于保存创建的套接字对；
*       socketpair函数建立一对匿名的已连接的套接字，建立的两个套接字描述符会放在sv[0]和sv[1]中。既可以从sv[0]写入sv[1]读出，又可以从sv[1]读入sv[0]写出，如果没有写入就读出则会生阻塞。用途：用来创建全双工通道，不过只局限于父子进程之间。
*/
		//struct pollfd 
		//{
		//	int fd;        /* 文件描述符 */
		//	short events; /* 等待的事件 */
		//	short revents; /* 实际发生了的事件 */
       //};


#define NQ		3		/* number of queues */
#define MAXMSZ	512		/* maximum message size */
#define KEY		0x123	/* key for first message queue */

struct threadinfo {
	int qid;
	int fd;
};

struct mymesg {
	long mtype;
	char mtext[MAXMSZ];
};

void *
helper(void *arg)
{
	int					n;
	struct mymesg		m;
	struct threadinfo	*tip = arg;

	for(;;) {
		memset(&m, 0, sizeof(m));
		if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)
		{
			err_sys("msgrcv error");
		}
		if (write(tip->fd, m.mtext, n) < 0)
		{
			err_sys("write error");
		}
	}
}

int
main()
{
	int					i, n, err;
	int					fd[2];
	int					qid[NQ];
	struct pollfd		pfd[NQ];
	struct threadinfo	ti[NQ];
	pthread_t			tid[NQ];
	char				buf[MAXMSZ];

	for (i = 0; i < NQ; i++) {
		if ((qid[i] = msgget((KEY+i), IPC_CREAT|0666)) < 0)
		{
			err_sys("msgget error");
		}

		printf("queue ID %d is %d\n", i, qid[i]);

		//AF_UNIX 域套接字
		if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
		{
			err_sys("socketpair error");
		}
		//poll监控 读出
		pfd[i].fd = fd[0];
		pfd[i].events = POLLIN;  //监控可读
		//从线程中往里写
		ti[i].qid = qid[i];
		ti[i].fd = fd[1];

		if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0)
		{
			err_exit(err, "pthread_create error");
		}
	}

	for (;;) {
		printf("pollo begin\n");
		
/*
	int poll(struct pollfd *fds, nfds_t nfds, int timeout);
	timeout  参数设置为-1时，表示永远阻塞等待。有事件的时后，返回
	          0表示立即返回，不阻塞。
	          大于0时，表示等待指定数目的毫秒数。
*/
		if (poll(pfd, NQ, -1) < 0)
		{
			err_sys("poll error");
		}
		printf("pollo end\n");
		for (i = 0; i < NQ; i++) 
		{
			if (pfd[i].revents & POLLIN) {
				if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
				{
					err_sys("read error");
				}
				buf[n] = 0;
				printf("queue id %d, message %s\n", qid[i], buf);
			}
		}
	}

	exit(0);
}
