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
//	2��msgsnd����
//	�ú�����������Ϣ��ӵ���Ϣ�����С�����ԭ��Ϊ��
//	int msgsend(int msgid, const void *msg_ptr, size_t msg_sz, int msgflg);
//	msgid����msgget�������ص���Ϣ���б�ʶ����
//
//	msg_ptr��һ��ָ��׼��������Ϣ��ָ�룬������Ϣ�����ݽṹȴ��һ����Ҫ��
//	ָ��msg_ptr��ָ�����Ϣ�ṹһ��Ҫ����һ�������ͳ�Ա������ʼ�Ľṹ�壬���պ������������Ա��ȷ����Ϣ�����͡�������Ϣ�ṹҪ�����������
//	struct my_message{
//    long int message_type;
//    /* The data you wish to transfer*/
//	};
//	msg_sz��msg_ptrָ�����Ϣ�ĳ��ȣ�ע������Ϣ�ĳ��ȣ������������ṹ��ĳ��ȣ�Ҳ����˵msg_sz�ǲ�������������Ϣ���ͳ�Ա�����ĳ���

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
 
	//������Ϣ����
	//msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	key_t msgkey = getMsgKey();
	msgid = msgget(msgkey, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
 
	//����Ϣ������д��Ϣ��ֱ��д��end
	while(running)
	{
		//��������
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);
		data.msg_type = 1;    //ע��2
		strcpy(data.text, buffer);
		//����з�������
		//msgflg
		//0������Ϣ������ʱ��msgsnd����������ֱ����Ϣ��д����Ϣ����
		//IPC_NOWAIT������Ϣ����������ʱ��msgsnd�������ȴ���������
		//IPC_NOERROR�������͵���Ϣ����size�ֽڣ���Ѹ���Ϣ�ضϣ��ضϲ��ֽ����������Ҳ�֪ͨ���ͽ��̡�
		if(msgsnd(msgid, (void*)&data, MAX_TEXT, 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		//����end��������
		if(strncmp(buffer, "end", 3) == 0)
		{
			running = 0;
		}
		sleep(1);
	}
	exit(EXIT_SUCCESS);
}

