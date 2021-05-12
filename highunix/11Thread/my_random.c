#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>


/* 
 *  测试随机数,种子是随机的，得出的随机数是固定的 
 *   */ 
void testRandom(){  
    srandom(1);  
    printf("RAND_MAX=%d\n",RAND_MAX);  
    for(int i=0;i<10;i++){  
        printf("i=%d random value=%ld \n",i+1,random());  
    }  
}  
  
/* 
 *  测试随机数,种子是随机的，得出的随机数是不固定的 
 *   */  
void testRandom2(){  
    srandom(time(NULL));  
    printf("RAND_MAX=%d\n",RAND_MAX);  
    for(int i=0;i<10;i++){  
        printf("i=%d random value=%ld \n",i+1,random());  
    }  
} 

int main(int argc, char *argv[]) {
	testRandom();
	testRandom2();
	printf("PTHREAD_BARRIER_SERIAL_THREAD=%d\n",PTHREAD_BARRIER_SERIAL_THREAD);
}
