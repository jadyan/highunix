#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>

#define BUFLEN		128

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


//extern int connect_retry(int, int, int, const struct sockaddr *,	socklen_t);


#define MAXSLEEP 128

int
connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
    int numsec, fd;

    /*
     * Try to connect with exponential backoff.
     */
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
        if ((fd = socket(domain, type, protocol)) < 0)
            return(-1);
        if (connect(fd, addr, alen) == 0) {
            /*
             * Connection accepted.
             */
            return(fd);
        }
        close(fd);

        /*
         * Delay before trying again.
         */
        if (numsec <= MAXSLEEP/2)
            sleep(numsec);
    }
    return(-1);
}

void
print_uptime(int sockfd)
{
	int		n;
	char	buf[BUFLEN];

	while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
		write(STDOUT_FILENO, buf, n);
	if (n < 0)
		err_sys("recv error");
}

int
main(int argc, char *argv[])
{
	struct addrinfo	*ailist, *aip;
	struct addrinfo	hint;
	int				sockfd, err;

	if (argc != 2)
		err_quit("usage: ruptime hostname");
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	//if ((err = getaddrinfo(argv[1], "9090", &hint, &ailist)) != 0)
	if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
	{
		err_quit("getaddrinfo error: %s", gai_strerror(err));
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next) 
	{
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		printf("************************\n\n");
		if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0, aip->ai_addr, aip->ai_addrlen)) < 0) 
		{
			err = errno;
		} else {
			print_uptime(sockfd);
			exit(0);
		}
	}
	err_exit(err, "can't connect to %s", argv[1]);
}
