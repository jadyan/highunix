#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
	11.6.1 互斥量
	page 321
	
	int pthread_mutex_trylock( pthread_mutex_t *mutex );
	//返回值
	//函数成功返回0。任何其他返回值都表示错误。
	//函数pthread_mutex_trylock是pthread_mutex_lock的非阻塞版本。
	//如果mutex参数所指定的互斥锁已经被锁定的话，调用pthread_mutex_trylock函数不会阻塞当前线程，而是立即返回一个值来描述互斥锁的状况。




/*
	该程序有些问题，在没有调用foo_alloc ，或者已经foo_rele后，线程调用foo_hold 会出现问题
*/

struct foo {
	int             f_count;
	pthread_mutex_t f_lock;
	int             f_id;
	/* ... more stuff here ... */
};

//使用前
struct foo *foo_alloc(int id) /* allocate the object */
{
	struct foo *fp;

	//动态申请互斥量
	if ((fp = malloc(sizeof(struct foo))) != NULL) 
	{
		fp->f_count = 1;
		fp->f_id = id;
		//初始化互斥量
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
		{
			free(fp);
			return(NULL);
		}
		/* ... continue initialization ... */
	}
	return(fp);
}


//使用中
void foo_hold(struct foo *fp) /* add a reference to the object */
{
	//对互斥量进行枷锁，调用线程会被阻塞，知道被解锁
	pthread_mutex_lock(&fp->f_lock);
	//当前线程使用 ，计数器+1
	fp->f_count++;                       
	pthread_mutex_unlock(&fp->f_lock);
}

	
//使用后
void foo_rele(struct foo *fp) /* release a reference to the object */
{
	pthread_mutex_lock(&fp->f_lock);
	if (--fp->f_count == 0)   //当前线程 把计数器-1，当没有线程在引用的时候销毁锁pthread_mutex_destroy
	{ /* last reference */
		pthread_mutex_unlock(&fp->f_lock);
		//在free之前 释放互斥锁，需要调用pthread_mutex_destroy
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		printf("call pthread_mutex_unlock\n");
		pthread_mutex_unlock(&fp->f_lock);
	}
}


void printfoo( const char *strinfo,const struct foo * fp)
{
	printf("%s:::: fp->f_count = %d\n", strinfo, fp->f_count);
}

int main()
{
    struct foo * fp;
 
printf("create\n");
	pthread_t pid = pthread_self();
    if((fp = foo_alloc(pid)) == NULL)
    {
        printf("foo_alloc error\n");
        return -1;
    }
printfoo("create end", fp);

    //printf("After foo_alloc, the fp->f_count = %d\n", fp->f_count);

    foo_hold(fp);
printfoo("After foo_hold 1", fp);
    //printf("After foo_hold, the fp->f_count = %d\n", fp->f_count);
 
    foo_hold(fp);
printfoo("After foo_hold 2", fp);
    //printf("After foo_hold, the fp->f_count = %d\n", fp->f_count);
 
    foo_rele(fp);
printfoo("\nAfter foo_rele 1", fp);
    //printf("After foo_rele, the fp->f_count = %d\n", fp->f_count);
 
    foo_rele(fp);
printfoo("\nAfter foo_rele 2", fp);
    //printf("After foo_rele, the fp->f_count = %d\n", fp->f_count);
 
    foo_rele(fp);	
printfoo("\nAfter foo_rele 3", fp);
    //printf("After foo_rele, the fp->f_count = %d\n", fp->f_count);
/*
	重复调用pthread_mutex_unlock 解锁 是ok的
*/
    foo_rele(fp);
printfoo("\nAfter foo_rele 4", fp);

    foo_rele(fp);
printfoo("\nAfter foo_rele 5", fp);
 
    return 0;
}
