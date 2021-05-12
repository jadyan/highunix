#include "apue.h"

#define	BUFFSIZE	1024
/*
	������δ������о�
*/
/*
	https://blog.csdn.net/aisxyz/article/details/84910394
	POSIX.1 ��Ϊ������ 6 ������ҵ�����йص��źš�
	* SIGCHLD���ӽ�����ֹͣ����ֹ��
	* SIGCONT�����������ֹͣ����ʹ��������У�������ԡ�
	* SIGSTOP��ֹͣ�źţ����ܱ���׽����ԣ���
	* SIGTSTP������ʽֹͣ�źš�
	* SIGTTIN����̨�������Ա�������նˡ�
	* SIGTTOU����̨�������Աд�����նˡ�
	�� SIGCHLD ���⣬�����Ӧ�ó��򲢲�������Щ�źţ�����ʽ shell ��ͨ���ᴦ����Щ�źŵ����й���������������ַ���ͨ���� Ctrl+Z��ʱ��SIGTSTP �ͱ�����ǰ̨����������н��̣�
	��֪ͨ shell ��ǰ̨���̨�ָ�����һ����ҵʱ��shell �������ҵ�е����н��̷��� SIGCONT �źš�����һ��ֹͣ�Ľ��̲���һ�� SIGCONT �ź�ʱ���ý��̾ͼ�������ʹ���ź��Ǳ���������Եġ�
	�������������ʾ�˵�һ����������ҵ����ʱͨ����ʹ�õĹ淶�������С��ó���ֻ�ǽ����׼���븴�Ƶ���׼����������źŴ����������ע����ʽ�����˹�����Ļ�ĳ����� VI����ִ�еĵ��Ͳ�����
		
		
	$ ./jobCtrlDemo.out
	abcde                                      # ��ȡ����
	abcde                                      # ���
	^Zsend SIGTSTP                             # Ctrl-Z ���� SIGTSTP �źŹ������
	[1]+  Stopped                 ./jobCtrlDemo.out
	$ ps                                       # �鿴���� ID
	   PID TTY          TIME CMD
	 23893 pts/1    00:00:00 jobCtrlDemo.out
	 23914 pts/1    00:00:00 ps
	 82002 pts/1    00:00:05 bash
	$ 
	$ kill -SIGCONT 23893                      # ���� SIGCONT �ź�ʹ���̼�������
	received SIGCONT
	[1]+  Stopped                 ./jobCtrlDemo.out
*/


static void
sig_tstp(int signo)	/* signal handler for SIGTSTP */
{
	sigset_t	mask;

	/* ... move cursor to lower left corner, reset tty mode ... */

	/*
	 * Unblock SIGTSTP, since it's blocked while we're handling it.
	 */
	sigemptyset(&mask);
	sigaddset(&mask, SIGTSTP);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	signal(SIGTSTP, SIG_DFL);	/* reset disposition to default */

	kill(getpid(), SIGTSTP);	/* and send the signal to ourself */

	/* we won't return from the kill until we're continued */

	signal(SIGTSTP, sig_tstp);	/* reestablish signal handler */

	/* ... reset tty mode, redraw screen ... */
}

int
main(void)
{
	int		n;
	char	buf[BUFFSIZE];

	/*
	 * Only catch SIGTSTP if we're running with a job-control shell.
	 */
	if (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
		signal(SIGTSTP, sig_tstp);

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");

	if (n < 0)
		err_sys("read error");

	exit(0);
}
