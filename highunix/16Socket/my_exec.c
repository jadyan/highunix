#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * 打印到界面上，替换后的 标准输入输出没有变 说明进程中的标准输出没有改变
 * */
int main(void)
{
    printf("entering main process---\n");
    execl("/bin/ls","ls","-l",NULL);
    printf("exiting main process ----\n");
    return 0;
}
