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

void* task(void*);		//�̺߳���

int main(){
	pthread_cond_init(&cond,NULL);			//��ʼ����������
	pthread_mutex_init(&work_mutex,NULL);	//��ʼ��������
	pthread_t* thread_handles;
	printf("��ָ����Ҫ�������߳�����");
	scanf("%d",&thread_count);
	thread_handles=(pthread_t*)malloc(thread_count*sizeof(pthread_t));
	for(int i=0;i<thread_count;i++)
		pthread_create(&thread_handles[1],NULL,task,(void*)i);
	//�ҳ����Ա�5��������,�̺߳�������+8����,����ӡ 
	while(flag<100){
		flag++;
		if(flag%5==0)
			pthread_cond_signal(&cond);
		else
			printf("���߳�->��ӡ��������%d\n",flag);
		Sleep(10);
	}
	for(int i=0;i<thread_count;i++)
		pthread_join(thread_handles[i],NULL);
	free(thread_handles);
	pthread_cond_destroy(&cond);
	return 0;
}


//������,������+8����ӡ 
void* task(void* rank){
	long my_rank;
	memcpy(&my_rank,(void*)&rank,4);
	while(1){
		if(flag==100)
			break;
		pthread_mutex_lock(&work_mutex);
		pthread_cond_wait(&cond,&work_mutex);       //����ź�֮ǰ�������»�û�����
		printf("�߳�%d->�������������%d\n",my_rank,flag+8);
		pthread_mutex_unlock(&work_mutex);          //��Ҫ�ڴ˴��ͷŻ�����
		Sleep(100);
	}
	pthread_cond_broadcast(&cond);					//�������,�������н��� 
	return NULL;
}

