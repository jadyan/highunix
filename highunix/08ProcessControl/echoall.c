#include "apue.h"


/*
 * Linux C中environ 变量是一个char** 类型，存储着系统的环境变量。
 * 因为environ是一个全局的外部变量，所以切记使用前要用extern关键字进行声明，然后在使用。
 *
 * unistd.h头文件中声明了这个变量，所以也可以将unist.h进行include，这个时候就不用再对environ进行extern声明了（应为unistd.h中已经声明了）
 * */
int main(int argc, char *argv[])
{
	int			i;
	char		**ptr;
	extern char	**environ;

	for (i = 0; i < argc; i++)		/* echo all command-line args */
	{
		printf("argv[%d]: %s\n", i, argv[i]);
	}

	for (ptr = environ; *ptr != 0; ptr++)	/* and all env strings */
	{
		printf("__\\__%s\n", *ptr);
	}

	exit(0);
}
