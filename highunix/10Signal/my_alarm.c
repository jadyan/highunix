#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
void handler()
{
    printf("hello\n");
}

main()
{
    int i;
    signal(SIGALRM, handler);
    for(i = 1; i < 1000000000; i++)
    {
		if (i %10==0)
		{
			alarm(5);
		}
        printf("sleep %d ...\n", i);
        sleep(1);
    }
}

