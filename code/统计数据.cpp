#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<bits/stdc++.h>
#include<time.h>
#include<omp.h>
using namespace std;

long long data_count;			//���ݸ���
double data[100000];			//�洢���ݵ�����
int bin_count=5;				//Ͱ�ĸ���
int bin_maxes[6]={0,1,2,3,4,5};	//Ͱ�ı߽�����
long long res[5]={0};			//ͳ�ƽ������ 
 
int main(){
	printf("���������ݸ�����");
	scanf("%ld",&data_count);
	srand(time(NULL));
	int num=0;
	for(int i=0;i<data_count;i++)
		data[i]=1.0*rand()/RAND_MAX*(5-0);	//�����������[0,5]�����������
	#pragma omp parallel for
	for(int i=0;i<data_count;i++){
		for(int j=0;j<bin_count;j++){
			#pragma omp critical
			if(data[i]>=bin_maxes[j]&&data[i]<bin_maxes[j+1])
				res[j]++;
		}
	}
	for(int i=0;i<bin_count;i++)
		printf("��%d-%d֮�������%ld��\n",i,i+1,res[i]);
}


