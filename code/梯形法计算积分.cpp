#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<malloc.h>
#include<bits/stdc++.h>
#include<semaphore.h>
using namespace std;
long long n=400000;				//С���εĸ��� 
int thread_count=8;				//�߳���
double a;						//��߽�
double b;						//�ұ߽�
double sum;						//����ܺ�
int flag;
long long deal=n/thread_count;	//ÿ���̼߳���С���ε���Ŀ
double h=(b-a)/n;				//ÿ��С���εĸ� 
pthread_mutex_t work_mutex;		//������ 
sem_t semaphore;				//�ź��� 

void* calculate(void*);			//�̺߳���
double f(double);				//���ֺ���

int main(){
	printf("������������䣺");
	scanf("%lf %lf",&a,&b);
	h=(b-a)/n;
	pthread_mutex_init(&work_mutex,NULL);		//��ʼ��������
	sem_init(&semaphore,0,1);					//��ʼ���ź��� 
	pthread_t* thread_handles;
	thread_handles=(pthread_t*)malloc(thread_count*sizeof(pthread_t));
	for(int i=0;i<thread_count;i++)
		pthread_create(&thread_handles[i],NULL,calculate,(void*)i);
	for(int i=0;i<thread_count;i++)
		pthread_join(thread_handles[i],NULL);
	printf("Our estimate of area = %.5lf\n",sum);
	free(thread_handles);
}

//���ֺ���f(x)=4*x*x+5*x+7
double f(double x){
	return 4*x*x+5*x+7;
}

/*
//�̺߳���(æ�ȴ�) 
void* calculate(void* num){
	long rank;
	memcpy(&rank,(void*)&num,4);
	double my_sum=0.0;
	double x1,x2,x;
	double begin=a+rank*deal*h;		//ÿ���̼߳����������ʼλ�� 
	double end=a+(rank+1)*deal*h;	//ÿ���̼߳��������ĩ��λ�� 
	for(int i=1;i<=deal;i++){
		while(flag!=rank);		//æ�ȴ�
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
//�̺߳���(������)
void* calculate(void* num){
	long rank;
	memcpy(&rank,(void*)&num,4);
	double my_sum=0.0;
	double x1,x2,x;
	double begin=a+rank*deal*h;		//ÿ���̼߳����������ʼλ�� 
	double end=a+(rank+1)*deal*h;	//ÿ���̼߳��������ĩ��λ�� 
	for(int i=1;i<=deal;i++){
		x1=begin+(i-1)*h;
		x2=begin+i*h;
		my_sum+=((f(x1)+f(x2))*h/2.0);
	}
	pthread_mutex_lock(&work_mutex);	//���� 
	sum+=my_sum;
	pthread_mutex_unlock(&work_mutex);	//���� 
	return NULL;
}
*/


//�̺߳���(�ź���)
void* calculate(void* num){
	long rank;
	memcpy(&rank,(void*)&num,4);
	double my_sum=0.0;
	double x1,x2,x;
	double begin=a+rank*deal*h;		//ÿ���̼߳����������ʼλ�� 
	double end=a+(rank+1)*deal*h;	//ÿ���̼߳��������ĩ��λ��
	for(int i=1;i<=deal;i++){
		x1=begin+(i-1)*h;
		x2=begin+i*h;
		my_sum+=((f(x1)+f(x2))*h/2.0);
	}
	sem_wait(&semaphore);			//�ȴ��ź��� 
	sum+=my_sum;
	sem_post(&semaphore);			//�����ź��� 
	return NULL;
}


