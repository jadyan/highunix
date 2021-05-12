#include "apue.h"
#include <errno.h>
#include <fcntl.h>

char    buf[500000];


/*
头文件：#include <unistd.h>    #include <fcntl.h>

定义函数：
int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock * lock);

函数说明：
1、fcntl()用来操作文件描述词的一些特性.。
2、参数fd 代表欲设置的文件描述词,。
3、参数cmd 代表欲操作的指令。有以下几种情况:
F_DUPFD 用来查找大于或等于参数arg 的最小且仍未使用的文件描述词, 并且复制参数fd 的文件描述词. 执行成功则返回新复制的文件描述词. 请参考dup2(). F_GETFD 取得close-on-exec 旗标. 若此旗标的FD_CLOEXEC 位为0, 代表在调用exec()相关函数时文件将不会关闭.
F_SETFD 设置close-on-exec 旗标. 该旗标以参数arg 的FD_CLOEXEC 位决定.
F_GETFL 取得文件描述词状态旗标, 此旗标为open()的参数flags.
F_SETFL 设置文件描述词状态旗标, 参数arg 为新旗标, 但只允许O_APPEND、O_NONBLOCK 和O_ASYNC 位的改变, 其他位的改变将不受影响.
F_GETLK 取得文件锁定的状态.
F_SETLK 设置文件锁定的状态. 此时flcok 结构的l_type 值必须是F_RDLCK、F_WRLCK 或F_UNLCK. 如果无法建立锁定, 则返回-1, 错误代码为EACCES 或EAGAIN.
F_SETLKW 同F_SETLK 作用相同, 但是无法建立锁定时, 此调用会一直等到锁定动作成功为止. 若在等待锁定的过程中被信号中断时, 会立即返回-1, 错误代码为EINTR. 参数lock 指针为flock 结构指针, 定义如下
*/



//    void
//    set_fl(int fd, int flags) /* flags are file status flags to turn on */
//    {
//        int     val;
//
//		/*
//			F_GETLK 取得文件锁定的状态.
//		*/
//        if ((val = fcntl(fd, F_GETFL, 0)) < 0)
//        {
//            err_sys("fcntl F_GETFL error");
//        }
//
//        val |= flags;       /* turn on flags */
//
//		/*
//			F_SETFL 设置文件描述词状态旗标, 参数arg 为新旗标, 但只允许O_APPEND、O_NONBLOCK 和O_ASYNC 位的改变, 其他位的改变将不受影响.
//		*/
//        if (fcntl(fd, F_SETFL, val) < 0)
//        {
//            err_sys("fcntl F_SETFL error");
//        }
//    }


int main(void)
{
    int        ntowrite, nwrite;
    char    *ptr;

    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
    fprintf(stderr, "read %d bytes\n", ntowrite);

 /*
    //非组赛 所以就引入了O_NONBLOCK,在读不到数据时，返回-1，并且设置errno为EAGAIN,而读到结尾处，正常返回0.
    //
    //O_NONBLOCK       非阻塞I/O;如果read(2)调用没有可读取的数据,或者如果write(2)操作将阻塞,read或write调用返回-1和EAGAIN错误               　　　　       　　
    //
	//O_APPEND         强制每次写(write)操作都添加在文件大的末尾,相当于open(2)的O_APPEND标志         
    //
	// O_DIRECT        最小化或去掉reading和writing的缓存影响.系统将企图避免缓存你的读或写的数据.
    //
	//                 如果不能够避免缓存,那么它将最小化已经被缓存了的数 据造成的影响.如果这个标志用的不够好,将大大的降低性能                     
    //
	// O_ASYNC         当I/O可用的时候,允许SIGIO信号发送到进程组,例如:当有数据可以读的时候
*/
    set_fl(STDOUT_FILENO, O_NONBLOCK);    /* set nonblocking */



/*
	程序发出了9000多个write调用，但是只有500个真正输出了数据，其余的都只返回了错误。这种形式的循环称为轮询，在多用户系统上用它会浪费CPU时间。14.4节将介绍非阻塞描述符的I/O多路转接，这是进行这种操作的一种比较有效的方法。
	有时，可以将应用程序设计成使用多线程的（见第11章），从而避免使用非阻塞I/O。如若我们能在其他线程中继续进行，则可以允许单个线程在I/O调用中阻塞。
	这种方法有时能简化应用程序的设计（见第21章），但是，线程间同步的开销有时却可能增加复杂性，于是导致得不偿失的后果。
	stderr.out
*/
    ptr = buf;
    while (ntowrite > 0) {
        errno = 0;
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);
        fprintf(stderr, "nwrite = %d, errno = %d , ntowrite =%d \n", nwrite, errno, ntowrite);

        if (nwrite > 0) {
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }

    clr_fl(STDOUT_FILENO, O_NONBLOCK);    /* clear nonblocking */

    exit(0);
}
