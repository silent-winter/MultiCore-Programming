#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<malloc.h>
#include<bits/stdc++.h>
#include<semaphore.h>
#include<time.h>
#include<windows.h>
using namespace std;

int thread_count;
pthread_mutex_t work_mutex;
pthread_cond_t cond;
int flag=0;

void* task(void*);		//线程函数

int main(){
	pthread_cond_init(&cond,NULL);			//初始化条件变量
	pthread_mutex_init(&work_mutex,NULL);	//初始化互斥量
	pthread_t* thread_handles;
	printf("请指定需要开启的线程数：");
	scanf("%d",&thread_count);
	thread_handles=(pthread_t*)malloc(thread_count*sizeof(pthread_t));
	for(int i=0;i<thread_count;i++)
		pthread_create(&thread_handles[1],NULL,task,(void*)i);
	//找出可以被5整除的数,线程函数进行+8操作,并打印 
	while(flag<100){
		flag++;
		if(flag%5==0)
			pthread_cond_signal(&cond);
		else
			printf("主线程->打印数据流：%d\n",flag);
		Sleep(10);
	}
	for(int i=0;i<thread_count;i++)
		pthread_join(thread_handles[i],NULL);
	free(thread_handles);
	pthread_cond_destroy(&cond);
	return 0;
}


//任务函数,将数据+8并打印 
void* task(void* rank){
	long my_rank;
	memcpy(&my_rank,(void*)&rank,4);
	while(1){
		if(flag==100)
			break;
		pthread_mutex_lock(&work_mutex);
		pthread_cond_wait(&cond,&work_mutex);       //获得信号之前，会重新获得互斥锁
		printf("线程%d->输出任务处理结果：%d\n",my_rank,flag+8);
		pthread_mutex_unlock(&work_mutex);          //需要在此处释放互斥锁
		Sleep(100);
	}
	pthread_cond_broadcast(&cond);					//任务结束,唤醒所有进程 
	return NULL;
}

