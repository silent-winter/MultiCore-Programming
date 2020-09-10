#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<malloc.h>
#include<bits/stdc++.h>
#include<semaphore.h>
using namespace std;
long long n=400000;				//小梯形的个数 
int thread_count=8;				//线程数
double a;						//左边界
double b;						//右边界
double sum;						//面积总和
int flag;
long long deal=n/thread_count;	//每个线程计算小梯形的数目
double h=(b-a)/n;				//每个小梯形的高 
pthread_mutex_t work_mutex;		//互斥量 
sem_t semaphore;				//信号量 

void* calculate(void*);			//线程函数
double f(double);				//积分函数

int main(){
	printf("请输入积分区间：");
	scanf("%lf %lf",&a,&b);
	h=(b-a)/n;
	pthread_mutex_init(&work_mutex,NULL);		//初始化互斥量
	sem_init(&semaphore,0,1);					//初始化信号量 
	pthread_t* thread_handles;
	thread_handles=(pthread_t*)malloc(thread_count*sizeof(pthread_t));
	for(int i=0;i<thread_count;i++)
		pthread_create(&thread_handles[i],NULL,calculate,(void*)i);
	for(int i=0;i<thread_count;i++)
		pthread_join(thread_handles[i],NULL);
	printf("Our estimate of area = %.5lf\n",sum);
	free(thread_handles);
}

//积分函数f(x)=4*x*x+5*x+7
double f(double x){
	return 4*x*x+5*x+7;
}

/*
//线程函数(忙等待) 
void* calculate(void* num){
	long rank;
	memcpy(&rank,(void*)&num,4);
	double my_sum=0.0;
	double x1,x2,x;
	double begin=a+rank*deal*h;		//每个线程计算区间的起始位置 
	double end=a+(rank+1)*deal*h;	//每个线程计算区间的末端位置 
	for(int i=1;i<=deal;i++){
		while(flag!=rank);		//忙等待
		x1=begin+(i-1)*h;
		x2=begin+i*h;
		my_sum+=((f(x1)+f(x2))*h/2.0);
		flag=(flag+1)%thread_count;
	}
	sum+=my_sum;
	return NULL;
}
*/

/*
//线程函数(互斥量)
void* calculate(void* num){
	long rank;
	memcpy(&rank,(void*)&num,4);
	double my_sum=0.0;
	double x1,x2,x;
	double begin=a+rank*deal*h;		//每个线程计算区间的起始位置 
	double end=a+(rank+1)*deal*h;	//每个线程计算区间的末端位置 
	for(int i=1;i<=deal;i++){
		x1=begin+(i-1)*h;
		x2=begin+i*h;
		my_sum+=((f(x1)+f(x2))*h/2.0);
	}
	pthread_mutex_lock(&work_mutex);	//加锁 
	sum+=my_sum;
	pthread_mutex_unlock(&work_mutex);	//解锁 
	return NULL;
}
*/


//线程函数(信号量)
void* calculate(void* num){
	long rank;
	memcpy(&rank,(void*)&num,4);
	double my_sum=0.0;
	double x1,x2,x;
	double begin=a+rank*deal*h;		//每个线程计算区间的起始位置 
	double end=a+(rank+1)*deal*h;	//每个线程计算区间的末端位置
	for(int i=1;i<=deal;i++){
		x1=begin+(i-1)*h;
		x2=begin+i*h;
		my_sum+=((f(x1)+f(x2))*h/2.0);
	}
	sem_wait(&semaphore);			//等待信号量 
	sum+=my_sum;
	sem_post(&semaphore);			//发送信号量 
	return NULL;
}


