#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<bits/stdc++.h>
#include<time.h>
#include<omp.h>
using namespace std;

int matrix[10000][10000];		//10000*10000矩阵
double ave;						//矩阵均值 

int main(){
	srand(time(NULL));
	for(int i=0;i<10000;i++){
		for(int j=0;j<10000;j++)
			matrix[i][j]=rand()%1000;
	}
	int max_element=matrix[0][0],min_element=matrix[0][0];
	long double sum=0.0;
	#pragma omp parallel for
	for(int i=0;i<10000;i++){
		long double my_sum=0.0;
		for(int j=0;j<10000;j++){
			if(matrix[i][j]>max_element)
				max_element=matrix[i][j];
			if(matrix[i][j]<min_element)
				min_element=matrix[i][j];
			my_sum+=matrix[i][j];
		}
		#pragma omp critical
		sum+=my_sum;
	}
	ave=sum/100000000.0;
	printf("最大元素为：%d\n",max_element);
	printf("最小元素为：%d\n",min_element);
	printf("均值为：%lf\n",ave);
}

