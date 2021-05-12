#include<unistd.h>
int main()
{
	//execlp("ls","ls","-al","/etc/passwd",(char *)0);
	execlp("ls","-al","/etc/passwd",(char *)0);
	return 0 ;
}
