#include "apue.h"
#if defined(SOLARIS)
#include <netinet/in.h>
#endif
#include <netdb.h>
#include <arpa/inet.h>
#if defined(BSD)
#include <sys/socket.h>
#include <netinet/in.h>
#endif

//https://baike.baidu.com/item/getaddrinfo?fr=aladdin
/*
	getaddrinfo函数能够处理名字到地址以及服务到端口这两种转换，返回的是一个sockaddr结构的链表而不是一个地址清单。这些sockaddr结构随后可由套接口函数直接使用。
*/
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
}

int
main(int argc, char *argv[])
{
	struct addrinfo		*ailist, *aip;
	struct addrinfo		hint;
	struct sockaddr_in	*sinp;
	const char 			*addr;
	int 				err;
	char 				abuf[INET_ADDRSTRLEN];

	if (argc != 3)
		err_quit("usage: %s nodename service", argv[0]);
	//（1）ai_flags;
	//（2）ai_family;
	//（3）ai_socktype;
	//（4）ai_protocol
	//AI_ADDRCONFIG: 查询配置的地址类型(IPv4或IPv6).
	//AI_ALL: 查找IPv4和IPv6地址(仅用于AI_V4MAPPED).
	//AI_CANONNAME: 需要一个规范名(而不是别名).
	//AI_NUMERICHOST: 以数字格式返回主机地址.
	//AI_NUMERICSERV: 以端口号返回服务.
	//AI_PASSIVE: socket地址用于监听绑定.
	//AI_V4MAPPED: 如果没有找到IPv6地址, 则返回映射到IPv6格式的IPv6地址.
	//除以上字段 其余全部为0 指针为NULL
	hint.ai_flags = AI_CANONNAME;      //page 482
	hint.ai_family = 0;
	hint.ai_socktype = 0;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	/*
		（1）hostname：是一个主机名或地址串
		（2）service：是一个服务名或十进制端口号数串
		（3）hints：一个空指针或指向某个addrinfo结构的指针，调用者在这个结构中填入关于期望返回的信息类型的暗示
	*/
	if ((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0)
	{
		err_quit("getaddrinfo error: %s", gai_strerror(err));
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		printf("\n\thost %s", aip->ai_canonname?aip->ai_canonname:"-");
		if (aip->ai_family == AF_INET) {
			sinp = (struct sockaddr_in *)aip->ai_addr;
			addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf,
			    INET_ADDRSTRLEN);
			printf(" address %s", addr?addr:"unknown");
			printf(" port %d", ntohs(sinp->sin_port));
		}
		printf("\n");
	}
	exit(0);
}
