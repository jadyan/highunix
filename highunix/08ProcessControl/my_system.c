#include "apue.h"
#include <sys/wait.h>


int main(int argc, char ** argv)
{
	const char * cmdstr="while true; do  echo \"****start....\"; ls; echo \"*****end.....\"; sleep 2; clear;done;";
	printf("%s\n", cmdstr);
	int ret = system(cmdstr);
	printf("返回值=%d", ret);
}
