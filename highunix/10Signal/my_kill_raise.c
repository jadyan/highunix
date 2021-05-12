#include  <stdio.h>
#include  <stdlib.h>
#include  <signal.h>

/*
	kill和raise是用来发送信号的:
	kill把信号发送给进程或进程组;
	raise把信号发送给(进程)自身.

*	raise(signo);

	等价于:
	kill(getpid(), signo);

	2. pid参数:

	kill函数中的pid参数, 它有以下4种情况:

	pid > 0: 将该信号发送给进程ID为pid的进程.
	pid == 0: 将该信号发送给与发送进程属于同一进程组的所有进程(不包括内核进程和init进程). 此时, 发送进程必须具有向这些进程发送信号的权限.
	pid < 0: 将该信号发给其进程组ID等于pid绝对值的所有进程(不包括内核进程和init进程). 此时, 发送进程必须具有向这些进程发送信号的权限.
	pid == -1: 将该信号发送给发送进程有权限向它们发送信号的系统上的所有进程.(不包括内核进程和init进程).
	3. signo参数:

	POSIX.1将编号为0的信号定义为空信号. 如果signo参数是0, 则kill仍执行正常的错误检查, 但不发送信号. 这被用来确定一个进程是否存在.
*/


int  main(  void  )
{
    pid_t childpid;
     int  status;
     int  retval;
    
    childpid  =  fork();
    if  (  - 1  ==  childpid )
    {
        perror(  " fork() "  );
        exit( EXIT_FAILURE );
    }
    else  if  (  0  ==  childpid )
    {
        puts(  " In child process "  );
        sleep(  100  ); // 让子进程睡眠，看看父进程的行为
        exit(EXIT_SUCCESS);
    }
    else
    {
        if (  0  ==  (waitpid( childpid,  & status, WNOHANG )))
        {
            retval  =  kill( childpid,SIGKILL );
            
            if  ( retval )
            {
                puts(  " kill failed. "  );
                perror(  " kill "  );
                waitpid( childpid,  & status,  0  );
            }
             else
            {
                printf(  " %d killed\n " , childpid );
            }
            
        }
    }
    
    exit(EXIT_SUCCESS);
}
