#include "apue.h"
#include <sys/msg.h>

#define MAXMSZ 512

struct mymesg {
	long mtype;
	char mtext[MAXMSZ];
};

int
main(int argc, char *argv[])
{
	key_t key;
	long qid;
	size_t nbytes;
	struct mymesg m;

	if (argc != 3) 
	{
		fprintf(stderr, "usage: sendmsg KEY message\n");
		exit(1);
	}
	/*
		 把参数 str 所指向的字符串根据给定的 base 转换为一个长整数（类型为 long int 型），
		 base 必须介于 2 和 36（包含）之间，或者是特殊值 0。
	*/
	key = strtol(argv[1], NULL, 0);
	if ((qid = msgget(key, 0)) < 0)
	{
		err_sys("can't open queue key %s", argv[1]);
	}
	memset(&m, 0, sizeof(m));
	strncpy(m.mtext, argv[2], MAXMSZ-1);
	nbytes = strlen(m.mtext);
	m.mtype = 1;
	if (msgsnd(qid, &m, nbytes, 0) < 0)
	{
		err_sys("can't send message");
	}
	exit(0);
}
