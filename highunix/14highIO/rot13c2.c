#include "apue.h"
#include <ctype.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

#define BSZ 4096
#define NBUF 8


/*
	https://yuchendoudou.gitee.io/2020/04/05/UNIX%E7%8E%AF%E5%A2%83%E9%AB%98%E7%BA%A7%E7%BC%96%E7%A8%8B-%E9%AB%98%E7%BA%A7IO/
	https://blog.csdn.net/u012206617/article/details/88875915
	AIO 背后的基本思想是允许进程发起很多 I/O 操作，而不用阻塞或等待任何操作完成。稍后或在接收到 I/O 操作完成的通知时，进程就可以检索 I/O 操作的结果。
*/
//struct aiocb {
//    int                     aio_fildes;     /*文件描述符    aio_fildes字段打开文件用于读或写的文件描述符*/
//    off_t                   aio_offset;     /*对于IO的文件偏移    在aio_offset指定的偏移处开始执行*/
//    volatile void           *aio_buf;       /*IO缓冲区    对于一个读，数据被复制到aio_buf指定的地址缓冲区处， 对于一个写，数据从这个缓冲区复制出来*/
//    size_t                  aio_nbytes;     /*转移的字节数  aio_nbytes字段包含读或写的字节数*/
//    int                     aio_reqprio;    /*优先级   aio_reqprio字段用于给应用提供一个建议异步IO请求顺序的方式*/
//    struct sigevent         aio_sigevent;   /*信号信息  aio_lio_opcode字段只能用于基于列表的异步IO*/
//    int                     aio_lio_opcode; /*对列表IO的操作*/
//}


//struct sigevent {
//    int             sigev_notify;                   /*通知类型*/
//    int             sigev_signo;                    /*信号编号*/
//    union sigval    sigev_value;                    /*通知参数*/
//    void (*sigev_notify_function)(union sigval);    /*通知函数*/
//    pthread_attr_t *sigev_notify_attributes;        /*通知属性*/
//}

enum rwop {
	UNUSED = 0,
	READ_PENDING = 1,
	WRITE_PENDING = 2
};

struct buf {
	enum rwop     op;
	int           last;
	struct aiocb  aiocb;
	unsigned char data[BSZ];
};

struct buf bufs[NBUF];

unsigned char
translate(unsigned char c)
{
	/* same as before */
	if (isalpha(c)) {
		if (c >= 'n')
			c -= 13;
		else if (c >= 'a')
			c += 13;
		else if (c >= 'N')
			c -= 13;
		else
			c += 13;
	}
	return(c);
}

int
main(int argc, char* argv[])
{
	int					ifd, ofd, i, j, n, err, numop;
	struct stat			sbuf;
	const struct aiocb	*aiolist[NBUF];
	off_t				off = 0;

	if (argc != 3)
	{
		err_quit("usage: rot13 infile outfile");
	}
	if ((ifd = open(argv[1], O_RDONLY)) < 0)
	{
		err_sys("can't open %s", argv[1]);
	}
	if ((ofd = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0)
	{
		err_sys("can't create %s", argv[2]);
	}
	if (fstat(ifd, &sbuf) < 0)
	{
		err_sys("fstat failed");
	}

	/* initialize the buffers */
	for (i = 0; i < NBUF; i++) {
		bufs[i].op = UNUSED;
		bufs[i].aiocb.aio_buf = bufs[i].data;    //IO缓冲区
		bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;  //但异步IO请求完成时，进程不被通知
		aiolist[i] = NULL;
	}

	numop = 0;
	for (;;) {
		for (i = 0; i < NBUF; i++) {
			switch (bufs[i].op) {
			case UNUSED:
				/*
				 * Read from the input file if more data
				 * remains unread.
				 */
				if (off < sbuf.st_size) 
				{
					//如果还有更多数据未读，则从输入文件读取
					bufs[i].op = READ_PENDING;
					bufs[i].aiocb.aio_fildes = ifd;    //文件描述符
					bufs[i].aiocb.aio_offset = off;    //对于IO的文件偏移
					off += BSZ;
					if (off >= sbuf.st_size)
						bufs[i].last = 1;
					bufs[i].aiocb.aio_nbytes = BSZ;
					/*
						异步IO请求就被操作系统排入到进程队列中。返回值和实际IO操作的结果没有任何联系。
						当IO操作被挂起时，我们需要小心确保AIO控制块和数据缓存稳定。它们的内存必须保持有效，并且在IO操作完成前我们我们重复使用它们。
					*/
					if (aio_read(&bufs[i].aiocb) < 0)
						err_sys("aio_read failed");
					aiolist[i] = &bufs[i].aiocb;
					numop++;
				}
				break;

			case READ_PENDING:
				if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
					continue;
				if (err != 0) {
					if (err == -1)
						err_sys("aio_error failed");
					else
						err_exit(err, "read failed");
				}

				/*
				 * A read is complete; translate the buffer
				 * and write it.
				 */
				////读取完成；转换缓冲区并写入
				if ((n = aio_return(&bufs[i].aiocb)) < 0)
					err_sys("aio_return failed");
				if (n != BSZ && !bufs[i].last)
					err_quit("short read (%d/%d)", n, BSZ);
				for (j = 0; j < n; j++)
					bufs[i].data[j] = translate(bufs[i].data[j]);
				bufs[i].op = WRITE_PENDING;
				bufs[i].aiocb.aio_fildes = ofd; //写入io
				bufs[i].aiocb.aio_nbytes = n;
				if (aio_write(&bufs[i].aiocb) < 0)
					err_sys("aio_write failed");
				/* retain our spot in aiolist */
				//保留我们在aiolist的位置
				break;

			case WRITE_PENDING:
				if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
					continue;
				if (err != 0) {
					if (err == -1)
						err_sys("aio_error failed");
					else
						err_exit(err, "write failed");
				}

				/*
				 * A write is complete; mark the buffer as unused.
				 */
				//写入完成：将缓冲区标记为未使用
				if ((n = aio_return(&bufs[i].aiocb)) < 0)
					err_sys("aio_return failed");
				if (n != bufs[i].aiocb.aio_nbytes)
					err_quit("short write (%d/%d)", n, BSZ);
				aiolist[i] = NULL;
				bufs[i].op = UNUSED;
				numop--;
				break;
			}
		}
		if (numop == 0) 
		{
			if (off >= sbuf.st_size)
				break;
		} else {
			//但我们完成程序，并发现我们仍然有异步操作，此时我们可以调用aio_suspend函数来阻塞到一个操作完成。
			if (aio_suspend(aiolist, NBUF, NULL) < 0)
				err_sys("aio_suspend failed");
		}
	}

	bufs[0].aiocb.aio_fildes = ofd;
	if (aio_fsync(O_SYNC, &bufs[0].aiocb) < 0)
		err_sys("aio_fsync failed");
	exit(0);
}
