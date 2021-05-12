#include "apue.h"
#include <fcntl.h>

//https://anmingyu11.gitbooks.io/unix/content/di-shi-si-zhang-gao-ji-i-o/143-ji-lu-suo.html


//	F_GETLK : 获取当前锁得状态
//	F_SETLK : 给当前文件上锁（非阻塞）。
//	F_SETLKW : 给当前文件上锁（阻塞，若当前文件正在被锁住，该函数一直阻塞）
//		
//	F_RDLCK : 读锁
//	F_WRLCK : 写锁
//	F_UNLCK : 无锁状态


//#define read_lock(fd, offset, whence, len) \
//            lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
//#define readw_lock(fd, offset, whence, len) \
//            lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
//#define write_lock(fd, offset, whence, len) \
//            lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
//#define writew_lock(fd, offset, whence, len) \
//            lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
//#define un_lock(fd, offset, whence, len) \
//            lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))



/*
*cmd:	//	F_GETLK : 获取当前锁得状态
		//	F_SETLK : 给当前文件上锁（非阻塞）。
		//	F_SETLKW : 给当前文件上锁（阻塞，若当前文件正在被锁住，该函数一直阻塞） W:代表wait、	
					   F_SETLKW 这个命令是F_SETLK的阻塞版本（命令名中的W表示等待（wait））。
					    如果所请求的读锁或写锁因另一个进程当前已经对所请求区域的某部分进行了加锁而不能被授予，那么调用进程会被置为休眠。
					    如果请求创建的锁已经可用，或者休眠由信号中断，则该进程被唤醒。
*
*/



/*
	我们设置l_start和l_whence指向文件的起始位置，并且指定长度（l_len）为0。（有多种方法可以指定文件起始处，但常用的方法是将l_start指定为0，l_whence指定为SEEK_SET。）
*/
//int
//lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
//{
//    struct flock    lock;
//
//    lock.l_type = type;     /* F_RDLCK, F_WRLCK, F_UNLCK   F_RDLCK（共享读锁）、F_WRLCK（独占性写锁）或 F_UNLCK（解锁一个区域）。*/
//    lock.l_start = offset;  /* byte offset, relative to l_whence  要加锁或解锁区域的起始字节偏移量*/
//    lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
//    lock.l_len = len;       /* #bytes (0 means to EOF) */
//
//    return(fcntl(fd, cmd, &lock));
//}



//	参数 whence 为下列其中一种:

    //SEEK_SET 参数offset 即为新的读写位置.
    //
    //SEEK_CUR 以目前的读写位置往后增加offset 个位移量.
    //
    //SEEK_END 将读写位置指向文件尾后再增加offset 个位移量. 当whence 值为SEEK_CUR 或
    //
    //SEEK_END 时, 参数offet 允许负值的出现.
    //
	//下列是教特别的使用方式:
	//1) 欲将读写位置移到文件开头时:lseek(int fildes, 0, SEEK_SET);
	//2) 欲将读写位置移到文件尾时:lseek(int fildes, 0, SEEK_END);
	//3) 想要取得目前文件位置时:lseek(int fildes, 0, SEEK_CUR);
		
	
	//SEEK_SET 将读写位置指向文件头后再增加offset个位移量。(当前位置为文件的开头)
	//SEEK_CUR 以目前的读写位置往后增加offset个位移量。
	//SEEK_END 将读写位置指向文件尾后再增加offset个位移量


//若一进程在某文件的16～32 字节区间有一把写锁，然后又试图在 16～32 字节区间加一把读锁，那么该请求将成功执行，原来的写锁会被替换为读锁。

//https://blog.csdn.net/u012349696/article/details/50791364
//int fcntl(int fd, int cmd); 
//int fcntl(int fd, int cmd, long arg); 
//int fcntl(int fd, int cmd, struct flock *lock);
//[描述]
//fcntl()针对(文件)描述符提供控制。参数fd是被参数cmd操作(如下面的描述)的描述符。针对cmd的值，fcntl能够接受第三个参数int arg。
//
//[返回值]
//fcntl()的返回值与命令有关。如果出错，所有命令都返回－1，如果成功则返回某个其他值。下列三个命令有特定返回值：F_DUPFD , F_GETFD , F_GETFL以及F_GETOWN。
//    F_DUPFD   返回新的文件描述符
//    F_GETFD   返回相应标志
//    F_GETFL , F_GETOWN   返回一个正的进程ID或负的进程组ID

 
//记录锁不被fork的子进程继承(PID不同)
//https://blog.csdn.net/iEearth/article/details/46728373  #记录锁 用之前先检查


static void lockabyte(const char *name, int fd, off_t offset)
{
	if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
	{
		err_sys("%s: writew_lock error", name);
	}
	printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}

int main(void)
{
	int		fd;
	pid_t	pid;

	/*
	 * Create a file and write two bytes to it.
	 */
	if ((fd = creat("templock", FILE_MODE)) < 0)
	{
		err_sys("creat error");
	}
	if (write(fd, "ab", 2) != 2)
	{
		err_sys("write error");
	}

	TELL_WAIT();
	if ((pid = fork()) < 0) 
	{
		err_sys("fork error");
	} 
	else if (pid == 0) 
	{			/* child */
		lockabyte("child", fd, 0);
		TELL_PARENT(getppid());
		WAIT_PARENT();
		lockabyte("child", fd, 1);
	} else {						/* parent */
		lockabyte("parent", fd, 1);
		TELL_CHILD(pid);
		WAIT_CHILD();
		lockabyte("parent", fd, 0);
	}
	exit(0);
}
