#include "apue.h"
#include <pwd.h>

/*
 * 可重入函数
 * page 263
 
	头文件：#include <pwd.h>    #include <sys/types.h>
	定义函数：struct passwd * getpwnam(const char * name);
	函数说明：getpwnam()用来逐一搜索参数name 指定的账号名称, 找到时便将该用户的数据以passwd 结构返回。passwd 结构请参考getpwent()。
	返回值：返回 passwd 结构数据, 如果返回NULL 则表示已无数据, 或有错误发
	getpwnam 非可重入
 * */
static void my_alarm(int signo)
{
	struct passwd	*rootptr;

	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL)
	{
			err_sys("getpwnam(root) error");
	}
	alarm(1);
}


/*
* SIGALRM当用alarm函数设置的定时器超时时，产生此信号。详细情况见10.10节。若由setitimer(2)函数设置的间隔时间已经超时时，也产生此信号。
*定义函数：unsigned int alarm(unsigned int seconds);
*函数说明：alarm()用来设置信号SIGALRM 在经过参数seconds 指定的秒数后传送给目前的进程. 如果参数seconds 为0, 则之前设置的闹钟会被取消, 并将剩下的时间返回.
**/
int main(void)
{
	struct passwd *ptr;
	signal(SIGALRM, my_alarm);	
	alarm(5);
	for ( ; ; )
	{
		//printf("alarm start\n");
		//alarm(10);   //未起作用
		//printf("alarm end\n");
	if ((ptr = getpwnam("jadyan")) == NULL)
		{
			err_sys("getpwnam error");
		}else{
		    //printf("name:%s\n", ptr->pw_name);
		    //printf("uid:%d\n", ptr->pw_uid);
		    //printf("home:%s\n", ptr->pw_dir);
		}
		
		if (strcmp(ptr->pw_name, "jadyan") != 0)
		{
			printf("return value corrupted!, pw_name = %s\n",ptr->pw_name);
		}
	}
}
