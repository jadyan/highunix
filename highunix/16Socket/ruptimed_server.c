#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>

#define BUFLEN	128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

void
print_family(struct addrinfo *aip)
{
	printf(" family ");
	switch (aip->ai_family) {
	case AF_INET:
		printf("****[inet ---- 2 IPv4]****");
		break;
	case AF_INET6:
		printf("[inet6]");
		break;
	case AF_UNIX:
		printf("[unix]");
		break;
	case AF_UNSPEC:
		printf("[unspecified]");
		break;
	default:
		printf("[unknown]");
	}
	printf("\n");
}

void
print_type(struct addrinfo *aip)
{
	printf(" type ");
	switch (aip->ai_socktype) {
	case SOCK_STREAM:
		printf("[stream]");
		break;
	case SOCK_DGRAM:
		printf("[datagram]");
		break;
	case SOCK_SEQPACKET:
		printf("[seqpacket]");
		break;
	case SOCK_RAW:
		printf("[raw]");
		break;
	default:
		printf("[unknown (%d)]", aip->ai_socktype);
	}
	printf("\n");
}

void
print_protocol(struct addrinfo *aip)
{
	printf(" [protocol] ");
	switch (aip->ai_protocol) {
	case 0:
		printf("[default]");
		break;
	case IPPROTO_TCP:
		printf("[TCP]");
		break;
	case IPPROTO_UDP:
		printf("[UDP]");
		break;
	case IPPROTO_RAW:
		printf("[raw]");
		break;
	default:
		printf("unknown (%d)", aip->ai_protocol);
	}
	printf("\n");
}

void
print_flags(struct addrinfo *aip)
{
	printf("flags");
	if (aip->ai_flags == 0) {
		printf(" 0");
	} else {
		if (aip->ai_flags & AI_PASSIVE)
			printf(" [****passive ----AI_PASSIVE:1被动的，用于bind，通常用于server socket****]");
		if (aip->ai_flags & AI_CANONNAME)
			printf(" [****canon ----AI_CANONNAME:2 用于返回主机的规范名称]****");
		if (aip->ai_flags & AI_NUMERICHOST)
			printf(" [****numhost ----AI_NUMERICHOST:4地址为数字串]****");
		if (aip->ai_flags & AI_NUMERICSERV)
			printf(" [numserv]");
		if (aip->ai_flags & AI_V4MAPPED)
			printf(" [v4mapped]");
		if (aip->ai_flags & AI_ALL)
			printf(" all");
	}
	printf("\n");
}


//extern int initserver(int, const struct sockaddr *, socklen_t, int);



int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd, err;
    int reuse = 1;

    if ((fd = socket(addr->sa_family, type, 0)) < 0)
    {
        return(-1);
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(int)) < 0)
    {
        goto errout;
    }
    if (bind(fd, addr, alen) < 0)
    {
        goto errout;
	}
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
        if (listen(fd, qlen) < 0)
        {
            goto errout;
        }
    }
    return(fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return(-1);
}

void serve(int sockfd)
{
	int		clfd;
	FILE	*fp;
	char	buf[BUFLEN];

	set_cloexec(sockfd);
	for (;;) {
		if ((clfd = accept(sockfd, NULL, NULL)) < 0)
		{
			syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
			exit(1);
		}
		set_cloexec(clfd);
//函数说明：popen()会调用fork()产生子进程，然后从子进程中调用/bin/sh -c 来执行参数command 的指令。
//参数type 可使用 "r"代表读取，"w"代表写入。依照此type 值，popen()会建立管道连到子进程的标准输出设备或标准输入设备，然后返回一个文件指针。
//随后进程便可利用此文件指针来读取子进程的输出设备或是写入到子进程的标准输入设备中
		if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
		{
			sprintf(buf, "error: %s\n", strerror(errno));
			send(clfd, buf, strlen(buf), 0);
		} else {
			while (fgets(buf, BUFLEN, fp) != NULL)
			{
				send(clfd, buf, strlen(buf), 0);
			}
			pclose(fp);
		}
		close(clfd);
	}
}

//父进程在fork函数的时候，子进程会拷贝跟父进程一样的地址空间，包括寄存器，文件描述符，堆，栈等。在一开始，父进程与子进程的文件描述符指向的是系统文件表中的同一项(包括文件状态，和文件偏移量)。
//当我们用execve执行其他程序的时候，全新的程序会替换子进程中的地址空间，数据段，堆栈，此时保存与父进程文件描述符也就不存在了，也无法进行关闭，这时候就需要FD_CLOEXEC,
// 表示子进程在执行exec的时候，该文件描述符就需要进行关闭。
//int set_cloexec(int fd)
//{
//    int     val;
//
//    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
//        return(-1);
//
//    val |= FD_CLOEXEC;      /* enable close-on-exec */
//
//    return(fcntl(fd, F_SETFD, val));
//}

int main(int argc, char *argv[])
{
	struct addrinfo	*ailist, *aip;
	struct addrinfo	hint;
	int				sockfd, err, n;
	char			*host;

	if (argc != 1)
	{
		err_quit("usage: ruptimed");
	}
	
	/*
		// sysconf 函数用来获取系统执行的配置信息。例如页大小、最大页数、cpu个数、打开句柄的最大个数等等
		//_SC_HOST_NAME_MAX：hostname最大长度，需小于_POSIX_HOST_NAME_MAX (255)
		//_SC_CHILD_MAX：每个user可同时运行的最大进程数
		//_SC_HOST_NAME_MAX：hostname最大长度，需小于_POSIX_HOST_NAME_MAX (255)
		//_SC_OPEN_MAX：一个进程可同时打开的文件最大数
		//_SC_PAGESIZE：一个page的大小，单位byte
		//_SC_PHYS_PAGES：物理内存总page数
		//_SC_AVPHYS_PAGES：当前可获得的物理内存page数
		//_SC_NPROCESSORS_CONF：配置的处理器个数
		//_SC_NPROCESSORS_ONLN：当前可获得的处理器个数
		//_SC_CLK_TCK：每秒对应的时钟tick数
	*/
	if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
	{
		n = HOST_NAME_MAX;	/* best guess */
	}
	if ((host = malloc(n)) == NULL)
	{
		err_sys("malloc error");
	}
	
	//返回本地主机的标准主机名。
	if (gethostname(host, n) < 0)
	{
		err_sys("gethostname error");
	}
	printf("主机名:%s\n",host);
	//daemonize("ruptimed");
	//daemonize("ruptimed_server"); 
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0) 
	{
		syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s",
		  gai_strerror(err));
		exit(1);
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next) 
	{
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		printf("\n\n");
		if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
		{
			serve(sockfd);
			exit(0);
		}
	}
	exit(1);
}
