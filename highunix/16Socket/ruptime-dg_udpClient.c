#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>

#define BUFLEN		128
#define TIMEOUT		20

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


void
sigalrm(int signo)
{
}

void
print_uptime(int sockfd, struct addrinfo *aip)
{
	int		n;
	char	buf[BUFLEN];

	buf[0] = 0;
	if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
	{
		err_sys("sendto error");
	}
	alarm(TIMEOUT);
	if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0) {
		if (errno != EINTR)
			alarm(0);
		err_sys("recv error");
	}
	alarm(0);
	write(STDOUT_FILENO, buf, n);
}

int
main(int argc, char *argv[])
{
	struct addrinfo		*ailist, *aip;
	struct addrinfo		hint;
	int					sockfd, err;
	struct sigaction	sa;

	if (argc != 2)
	{
		err_quit("usage: ruptime hostname");
	}
	sa.sa_handler = sigalrm;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) < 0)
	{
		err_sys("sigaction error");
	}
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
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
		
		if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0) 
		{
			err = errno;
		} else {
			print_uptime(sockfd, aip);
			exit(0);
		}
	}

	fprintf(stderr, "can't contact %s: %s\n", argv[1], strerror(err));
	exit(1);
}
