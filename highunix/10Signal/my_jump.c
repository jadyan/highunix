#include <stdio.h>
#include <setjmp.h>
 
//记录恢复的位置变量
jmp_buf buf;
 
void test() {
    
    printf("进入test()函数 \n");
    //跳转到buf位置变量记录的位置
    //void longjmp(jmp_buf env, int value)
    //如果value传递给longjmp零值，setjmp的返回值为1；否则，setjmp的返回值为value。
    longjmp(buf, 1);
    printf("返回test()函数 \n");
}
 
int main(int argc, const char * argv[]) {
 
    //setjmp(buf)如果在setjmp本函数体内返回，则返回0
    //如果通过longjmp函数返回，则返回非0值
    if(setjmp(buf)) 
    {
        //返回了非0值，通过longjmp函数跳转到setjump函数返回
        printf("main()... setjump()函数通过longjmp返回 \n");
    } else {
        //第一次调用setjmp时，必定正常执行完毕返回，即返回0值
        printf("main()... setjump()函数正常执行完毕返回 \n");
        test();
    }
    
    return 0;
}