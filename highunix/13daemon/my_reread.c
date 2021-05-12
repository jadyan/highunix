#define _XOPEN_SOURCE 600

#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <unistd.h>   
#include <fcntl.h>   
#include <time.h>   
#include <signal.h>   
#include <errno.h>   
#include <sys/resource.h>   
#include <sys/syslog.h>   
#include <sys/file.h>   
#include <sys/stat.h>   


/*
	注解：守护进程的编程规则
1：首先要调用umask将文件模式创建屏蔽字设置为0.由继承得来的文件模式创建屏蔽字可能会拒绝设置某些权限。
2：调用fork，然后使父进程退出（exit）。这样做实现了两点：第一，如果该守护进程是作为一条简单shell命令启动的，那么父进程终止使得shell认为这条命令已经执行完毕；第二，子进程继承了父进程的进程组ID，但具有一个新的进程ID，这就保证了子进程不是一个进程组的组长进程。这是setsid调用必要前���条件。
3：调用setsid以创建一个新会话。执行三个操作，（a）成为新会话的首进程，（b）成为一个新进程的组长进程，（c）没有控制终端。
4：将当前工作目录更改为根目录。进程活动时，其工作目录所在的文件系统不能卸下。一般需要将工作目录改变到根目录。对于需要转储核心，写运行日志的进程将工作目录改变到特定目录
5：关闭不再需要的文件描述符。进程从创建它的父进程那里继承了打开的文件描述符。如不关闭，将会浪费系统资源，造成进程所在的文件系统无法卸下以及引起无法预料的错误。
6：重定向0,1,2到/dev/null，使任何一个试图读标准输入，写标准输出和标准出错的程序库都不会产生任何效果。
*/

//创建锁文件的路径   
#define LOCKFILE "/var/run/daemon.pid"   
//锁文件的打开模式   
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)   
  
//输出错误信息并退出     
void error_quit(const char *str)    
{    
    fprintf(stderr, "%s\n", str);    
    exit(1);    
}   
  
//对文件fd加上记录锁   
int lockfile(int fd)  
{  
    struct flock fl;  
    fl.l_type = F_WRLCK;  
    fl.l_start = 0;  
    fl.l_whence = SEEK_SET;  
    fl.l_len = 0;  
    return fcntl(fd, F_SETLK, &fl);  
}  
  
//若程序已经运行，则返回1，否则返回0   
int already_running(void)  
{  
    int fd;  
    char buf[16];  
  
    //打开放置记录锁的文件   
    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);  
    if( fd < 0 )  
    {  
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));  
        exit(1);  
    }  
    //试图对文件fd加锁，   
    //如果加锁失败的话   
    if( lockfile(fd) < 0 )  
    {  
        //如果是因为权限不够或资源暂时不可用，则返回1   
        if( EACCES == errno ||  
            EAGAIN == errno )  
        {  
            close(fd);  
            return 1;  
        }  
        //否则，程序出错，写入一条错误记录后直接退出   
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));  
        exit(1);  
    }  
  
    //先将文件fd清空，然后再向其中写入当前的进程号   
    ftruncate(fd, 0);  
    sprintf(buf, "%ld", (long)getpid());  
    write(fd, buf, strlen(buf)+1);  
    return 0;  
}  
  
//将一个进程变为守护进程   
void daemonize(void)  
{  
    int i, fd0, fd1, fd2;  
    pid_t pid;  
    struct rlimit rl;  
    struct sigaction sa;  
  
    //见注解1   
    umask(0);  
  
    //获取最大的文件描述号   
    int temp;  
    temp = getrlimit(RLIMIT_NOFILE, &rl);  
    if( temp < 0 )  
        error_quit("can't get file limit");  
  
    //见注解2，   
    pid = fork();  
    if( pid < 0 )  
        error_quit("can't fork");  
    else if(pid != 0)  
        exit(0);  
  
    //见注解3   
    setsid();  
    sa.sa_handler = SIG_IGN;  
    sigemptyset(&sa.sa_mask);  
    sa.sa_flags = 0;  
    temp = sigaction(SIGHUP, &sa, NULL);  
    if( temp < 0 )  
        error_quit("can't ignore SIGHUP");  
  
    ////确保子进程不会有机会分配到一个控制终端   
    pid = fork();  
    if( pid < 0 )  
        error_quit("can't fork");  
    else if(pid != 0)  
        exit(0);  
  
    //见注解4   
    temp = chdir("/");  
    if( temp < 0 )  
        error_quit("can't change directoy to /");  
  
    //见注解5   
    if( rl.rlim_max == RLIM_INFINITY )  
        rl.rlim_max = 1024;  
    for(i=0; i<rl.rlim_max; i++)  
        close(i);  
  
    //见注解6   
    fd0 = open("/dev/null", O_RDWR);  
    fd1 = dup(0);  
    fd2 = dup(0);  
  
    if( fd0 != 0 ||  
        fd1 != 1 ||  
        fd2 != 2 )  
    {  
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d",  
            fd0, fd1, fd2);  
        exit(1);  
    }  
}  
  
//该主函数是我原创的，呵呵   
int main(void)  
{  
    //打开系统的日志文件   
    openlog("my test log: ", LOG_CONS, LOG_DAEMON);  
    daemonize();  
  
    //如果程序已经运行，则向记录文件中写入一句话，然后退出   
    if( already_running() )  
    {  
        syslog(LOG_ERR, "daemon alread running");  
        closelog();  
        return 1;  
    }  
  
    //向日志文件写入程序的开始（当前）时间，   
    //过100秒后，再向记录文件写入结束时间，然后结束程序   
    time_t tt = time(0);  
    syslog(LOG_INFO, "the log program start at: %s",   
        asctime(localtime(&tt)) );  
    sleep(100);  
    //pause()   
    tt = time(0);  
    syslog(LOG_INFO, "the log program end at: %s",   
        asctime(localtime(&tt)) );  
  
    //关闭日志文件   
    //虽然不关也没事，但为了和openlog配对，还是将它写上去吧   
    closelog();  
    return 0;  
}  