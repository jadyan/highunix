#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void printsigset(const sigset_t *set)
{
	int i;
	for (i = 1; i < 32; i++)
		if (sigismember(set, i) == 1)
		{
			putchar('1');
		}
		else
		{
			putchar('0');
		}
	puts("");
}

int main(void)
{
	sigset_t s, p;
	sigemptyset(&s);
	sigaddset(&s, SIGINT);
	
	sigprocmask(SIG_BLOCK, &s, NULL);
	/*
	忽略信号SIGQUIT
	*/
	signal(SIGQUIT, SIG_IGN);
	while (1)
    {
		sigpending(&p);
		printsigset(&p);
		sleep(1);
	}
	return 0;
}
