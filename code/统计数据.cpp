#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<bits/stdc++.h>
#include<time.h>
#include<omp.h>
using namespace std;

long long data_count;			//数据个数
double data[100000];			//存储数据的数组
int bin_count=5;				//桶的个数
int bin_maxes[6]={0,1,2,3,4,5};	//桶的边界数组
long long res[5]={0};			//统计结果数组 
 
int main(){
	printf("请输入数据个数：");
	scanf("%ld",&data_count);
	srand(time(NULL));
	int num=0;
	for(int i=0;i<data_count;i++)
		data[i]=1.0*rand()/RAND_MAX*(5-0);	//随机生成区间[0,5]的随机浮点数
	#pragma omp parallel for
	for(int i=0;i<data_count;i++){
		for(int j=0;j<bin_count;j++){
			#pragma omp critical
			if(data[i]>=bin_maxes[j]&&data[i]<bin_maxes[j+1])
				res[j]++;
		}
	}
	for(int i=0;i<bin_count;i++)
		printf("在%d-%d之间的数有%ld个\n",i,i+1,res[i]);
}


