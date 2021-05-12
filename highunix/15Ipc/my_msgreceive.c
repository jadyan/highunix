#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
 
struct msg_st
{
	long int msg_type;
	char text[BUFSIZ];
};

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



//	函数原型
//	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
//	数传入值

//	msqid
//		消息队列标识符
//	msgp
//		存放消息的结构体，结构体类型要与msgsnd函数发送的类型相同
//	msgsz
//		要接收消息的大小，不含消息类型占用的4个字节
//	msgtyp
//		0：接收第一个消息
//		>0：接收类型等于msgtyp的第一个消息
//		<0：接收类型等于或者小于msgtyp绝对值的第一个消息
//	msgflg
//		0: 阻塞式接收消息，没有该类型的消息msgrcv函数一直阻塞等待
//		IPC_NOWAIT：如果没有返回条件的消息调用立即返回，此时错误码为ENOMSG
//		IPC_EXCEPT：与msgtype配合使用返回队列中第一个类型不为msgtype的消息
//		IPC_NOERROR：如果队列中满足条件的消息内容大于所请求的size字节，则把该消息截断，截断部分将被丢弃

int main()
{
	int running = 1;
	int msgid = -1;
	struct msg_st data;
	long int msgtype = 1; //注意1
 
	//建立消息队列
	//msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	key_t msgkey = getMsgKey();
	msgid = msgget(msgkey, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	//从队列中获取消息，直到遇到end消息为止
	while(running)
	{
		//printf("ok:\n");
		//会阻塞在这里
		//msgtyp
		//0：接收第一个消息
		//>0：接收类型等于msgtyp的第一个消息
		//<0：接收类型等于或者小于msgtyp绝对值的第一个消息
		if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1)
		{
			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		//printf("ok2:\n");
		printf("You wrote: %s\n",data.text);
		//遇到end结束
		if(strncmp(data.text, "end", 3) == 0)
			running = 0;
	}
	//删除消息队列
	if(msgctl(msgid, IPC_RMID, NULL) == -1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}