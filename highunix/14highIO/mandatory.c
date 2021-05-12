#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>


/*
	建议锁，顾名思义，相对温柔一些，在对文件进行锁操作时，会检测是否已经有锁存在，并且尊重已有的锁，但是另外的进程还可以修改文件。在一般的情况下，内核和系统都不使用建议锁。
	强制性的锁是由内核执行的锁，当一个文件被上锁进行写入操作的时候，内核将阻止其他进程对其进行读写操作。
*/
/*
//O_RDONLY 以只读方式打开文件
//O_WRONLY 以只写方式打开文件
//O_RDWR 以可读写方式打开文件. 上述三种旗标是互斥的, 也就是不可同时使用, 但可与下列的旗标利用OR(|)运算符组合.
//O_CREAT 若欲打开的文件不存在则自动建立该文件.
//O_EXCL 如果O_CREAT 也被设置, 此指令会去检查文件是否存在. 文件若不存在则建立该文件, 否则将导致打开文件错误. 此外, 若O_CREAT 与O_EXCL 同时设置, 并且欲打开的文件为符号连接, 则会打开文件失败.
//O_NOCTTY 如果欲打开的文件为终端机设备时, 则不会将该终端机当成进程控制终端机.
//O_TRUNC 若文件存在并且以可写的方式打开时, 此旗标会令文件长度清为0, 而原来存于该文件的资料也会消失.
//O_APPEND 当读写文件时会从文件尾开始移动, 也就是所写入的数据会以附加的方式加入到文件后面.
//O_NONBLOCK 以不可阻断的方式打开文件, 也就是无论有无数据读取或等待, 都会立即返回进程之中.
//O_NDELAY 同O_NONBLOCK.
//O_SYNC 以同步的方式打开文件.
//O_NOFOLLOW 如果参数pathname 所指的文件为一符号连接, 则会令打开文件失败.
//O_DIRECTORY 如果参数pathname 所指的文件并非为一目录, 则会令打开文件失败


//S_IRWXU00700 权限, 代表该文件所有者具有可读、可写及可执行的权限.
//S_IRUSR 或S_IREAD, 00400 权限, 代表该文件所有者具有可读取的权限.
//S_IWUSR 或S_IWRITE, 00200 权限, 代表该文件所有者具有可写入的权限.
//S_IXUSR 或S_IEXEC, 00100 权限, 代表该文件所有者具有可执行的权限.
//S_IRWXG 00070 权限, 代表该文件用户组具有可读、可写及可执行的权限.
//S_IRGRP 00040 权限, 代表该文件用户组具有可读的权限.
//S_IWGRP 00020 权限, 代表该文件用户组具有可写入的权限.
//S_IXGRP 00010 权限, 代表该文件用户组具有可执行的权限.
//S_IRWXO 00007 权限, 代表其他用户具有可读、可写及可执行的权限.
//S_IROTH 00004 权限, 代表其他用户具有可读的权限
//S_IWOTH 00002 权限, 代表其他用户具有可写入的权限.
//S_IXOTH 00001 权限, 代表其他用户具有可执行的权限.
*/


//头文件：#include <sys/stat.h>   #include <unistd.h>
//
//定义函数：int fstat(int fildes, struct stat *buf);
//
//函数说明：fstat()用来将参数fildes 所指的文件状态, 复制到参数buf 所指的结构中(struct stat). 
//       Fstat()与stat()作用完全相同, 不同处在于传入的参数为已打开的文件描述词. 详细内容请参考stat().
//
//返回值：执行成功则返回0, 失败返回-1, 错误代码存于errno.

//
//https://www.cnblogs.com/yaowen/p/4801541.html
//struct stat  
//{   
//    dev_t       st_dev;     /* ID of device containing file -文件所在设备的ID*/  
//    ino_t       st_ino;     /* inode number -inode节点号*/    
//    mode_t      st_mode;    /* protection -保护模式?*/    
//    nlink_t     st_nlink;   /* number of hard links -链向此文件的连接数(硬连接)*/    
//    uid_t       st_uid;     /* user ID of owner -user id*/    
//    gid_t       st_gid;     /* group ID of owner - group id*/    
//    dev_t       st_rdev;    /* device ID (if special file) -设备号，针对设备文件*/    
//    off_t       st_size;    /* total size, in bytes -文件大小，字节为单位*/    
//    blksize_t   st_blksize; /* blocksize for filesystem I/O -系统块的大小*/    
//    blkcnt_t    st_blocks;  /* number of blocks allocated -文件所占块数*/    
//    time_t      st_atime;   /* time of last access -最近存取时间*/    
//    time_t      st_mtime;   /* time of last modification -最近修改时间*/    
//    time_t      st_ctime;   /* time of last status change - */    
//};  


//http://c.biancheng.net/cpp/html/311.html
//相关函数：chmod, stat, open, chown
//头文件：#include <sys/types.h>   #include <sys/stat.h>
//定义函数：int fchmod(int fildes, mode_t mode);
//函数说明：fchmod()会依参数mode 权限来更改参数fildes 所指文件的权限。参数fildes 为已打开文件的文件描述词。参数mode 请参考chmod ()。
//返回值：权限改变成功则返回0, 失败返回-1, 错误原因存于errno.

//错误代码：
//1、EBADF 参数fildes 为无效的文件描述词。
//2、EPERM 进程的有效用户识别码与欲修改权限的文件所有者不同, 而且也不具root 权限。
//3、EROFS 欲写入权限的文件存在于只读文件系统内。
//4、EIO I/O 存取错误。

//范例
//#include <sys/stat.h>
//#include <fcntl.h>
//main()
//{
//    int fd;
//    fd = open("/etc/passwd", O_RDONLY);
//    fchmod(fd, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
//    close(fd);
//}

/*
//	https://anmingyu11.gitbooks.io/unix/content/di-shi-si-zhang-gao-ji-i-o/143-ji-lu-suo.html
*/

int
main(int argc, char *argv[])
{
	int				fd;
	pid_t			pid;
	char			buf[5];
	struct stat		statbuf;

	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}
	//#define    FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
	/*
		执行完open 当前权限-rw-r--r-- 
	*/
	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
		err_sys("open error");
	if (write(fd, "abcdef", 6) != 6)
		err_sys("write error");

	/* turn on set-group-ID and turn off group-execute */
	if (fstat(fd, &statbuf) < 0)
		err_sys("fstat error");
	if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		err_sys("fchmod error");
	//执行完fchmod  fchmod -rw-r-Sr--

#if 1
	TELL_WAIT();

	if ((pid = fork()) < 0) 
	{
		err_sys("fork error");
	} 
	else if (pid > 0) 
	{	/* parent */
		/* write lock entire file */
		/*写但不阻塞*/
		if (write_lock(fd, 0, SEEK_SET, 0) < 0)
		{
			err_sys("write_lock error");
		}

		TELL_CHILD(pid);

		//等待子进程结束
		if (waitpid(pid, NULL, 0) < 0)
		{
			err_sys("waitpid error");
		}
	} else {				/* child */
		//printf("****child start\n");
		//父进程不调用TELL_CHILD 会阻塞在这里,这样保证父进程先执行
		WAIT_PARENT();		/* wait for parent to set lock */
		//printf("****child start2\n");
		set_fl(fd, O_NONBLOCK);

		/* first let's see what error we get if region is locked */
		/* 读锁 不阻塞*/
		if (read_lock(fd, 0, SEEK_SET, 0) != -1)	/* no wait */
		{
			err_sys("child: read_lock succeeded");
		}else{
			printf("read_lock of already-locked region returns %d\n",errno);
		}

		//可以写入(说明不支持强制锁)
		if (write(fd, "1213443", 6) != 6)
		{
			printf("。。。。。write error");
		}
		
		/* now try to read the mandatory locked file */
		if (lseek(fd, 0, SEEK_SET) == -1)
		{
			err_sys("lseek error");
		}
		if (read(fd, buf, 2) < 0)
		{
			err_ret("read failed (mandatory locking works)");
		}
		else
		{
			printf("read OK (no mandatory locking), buf = %2.2s\n",buf);
		}
	}
	exit(0);
#endif
}
