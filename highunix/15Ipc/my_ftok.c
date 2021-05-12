#define _XOPEN_SOURCE 700
#include <stdio.h>  
#include <sys/sem.h>  
#include <stdlib.h>  

int main()  
{  
    key_t semkey;  
    if((semkey = ftok("./myuclc.c", 1))<0)  
    {  
        printf("ftok failed\n");  
        exit(EXIT_FAILURE);  
    }     
    printf("ftok ok ,semkey = %d\n", semkey);  
    printf("ftok ok ,semkey = %d\n", ftok("./myuclc.c", 1));  
    printf("ftok ok ,semkey = %d\n", ftok("./myuclc.c", 2));  

    
    return 0;  
}
