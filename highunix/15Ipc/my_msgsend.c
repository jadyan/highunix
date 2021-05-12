#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
 
#define MAX_TEXT 512
struct msg_st
{
	long int msg_type;
	char text[MAX_TEXT];
};
 

//https://blog.csdn.net/guoping16/article/details/6584024
//https://blog.csdn.net/ljianhui/article/details/10287879
//	2、msgsnd函数
//	该函数用来把消息添加到消息队列中。它的原型为：
//	int msgsend(int msgid, const void *msg_ptr, size_t msg_sz, int msgflg);
//	msgid是由msgget函数返回的消息队列标识符。
//
//	msg_ptr是一个指向准备发送消息的指针，但是消息的数据结构却有一定的要求，
//	指针msg_ptr所指向的消息结构一定要是以一个长整型成员变量开始的结构体，接收函数将用这个成员来确定消息的类型。所以消息结构要定义成这样：
//	struct my_message{
//    long int message_type;
//    /* The data you wish to transfer*/
//	};
//	msg_sz是msg_ptr指向的消息的长度，注意是消息的长度，而不是整个结构体的长度，也就是说msg_sz是不包括长整型消息类型成员变量的长度

key_t getMsgKey()
{
	key_t semkey;
	if( (semkey = ftok("./Makefile", 1)) < 0)
	{
        printf("ftok failed\n");
        exit(EXIT_FAILURE);
	}
	printf("ftok ok ,semkey = %d\n", semkey);
	return semkey;
}

int main()
{
	int running = 1;
	struct msg_st data;
	char buffer[BUFSIZ];
	int msgid = -1;
 
	//建立消息队列
	//msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	key_t msgkey = getMsgKey();
	msgid = msgget(msgkey, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
 
	//向消息队列中写消息，直到写入end
	while(running)
	{
		//输入数据
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);
		data.msg_type = 1;    //注意2
		strcpy(data.text, buffer);
		//向队列发送数据
		//msgflg
		//0：当消息队列满时，msgsnd将会阻塞，直到消息能写进消息队列
		//IPC_NOWAIT：当消息队列已满的时候，msgsnd函数不等待立即返回
		//IPC_NOERROR：若发送的消息大于size字节，则把该消息截断，截断部分将被丢弃，且不通知发送进程。
		if(msgsnd(msgid, (void*)&data, MAX_TEXT, 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		//输入end结束输入
		if(strncmp(buffer, "end", 3) == 0)
		{
			running = 0;
		}
		sleep(1);
	}
	exit(EXIT_SUCCESS);
}

