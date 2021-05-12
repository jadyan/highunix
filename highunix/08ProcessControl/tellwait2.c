#include "apue.h"


/*
void TELL_WAIT(void)
{   
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    
    // Block SIGUSR1 and SIGUSR2, and save current signal mask 
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
        
        
    void WAIT_PARENT(void)
	{
	    while (sigflag == 0)
	        sigsuspend(&zeromask);  // and wait for parent 
	    sigflag = 0;

	    // Reset signal mask to original value 
	    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	        err_sys("SIG_SETMASK error");
	}
}
*/


static void charatatime(char *);

int
main(void)
{
	pid_t	pid;

	TELL_WAIT();

	if ((pid = fork()) < 0) 
    {
		err_sys("fork error");
	} else if (pid == 0)
    {
		/*
		  等待父进程先输出
        */
		WAIT_PARENT();		/* parent goes first */
		charatatime("output from child\n");
	} 
	else
    {
		charatatime("output from parent\n");
		TELL_CHILD(pid);
	}
	exit(0);
}

static void charatatime(char *str)
{
	char	*ptr;
	int		c;

	setbuf(stdout, NULL);			/* set unbuffered */
	for (ptr = str; (c = *ptr++) != 0; )
	{
		putc(c, stdout);
	}
}
