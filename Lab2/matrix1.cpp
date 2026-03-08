#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "unistd.h"
int **matrix;
//matrix
int **tmp;
int T,n; 
//T for num_of_threads, n for martix_dimension

void *thread_function(void *arg) //多线程分块
{
    int thd_no = *(int *)arg;
    int start = thd_no / sqrt((double)T); //起始位置块行列号
    int end = thd_no % (int)sqrt((double)T);  //结束位置块行列号
    int m = sqrt((double)(n*n/T)), i, j;
    for(i=start*m; i<(start+1)*m; i++)
    {
    	for(j=end*m; j<(end+1)*m; j++)
    		matrix[i][j] = tmp[j][i];
	}
    return NULL;
}

int main(int argc, const char * argv[]) {
    n = atoi(argv[1]);  //获取数据量（矩阵维数）
	T = atoi(argv[2]);  //获取线程数
	                     //为了计算和分析方便，本实验的线程数都取得完全平方数
						 //
	
	matrix = (int **)malloc(n*sizeof(int *));
    tmp = (int **)malloc(n*sizeof(int *));
    for(int i=0; i<n; i++) {
        matrix[i] = (int *)malloc(n*sizeof(int));
        tmp[i] = (int *)malloc(n*sizeof(int));
    }
    
    //init the matrix
    int cnt = 0;
    for(int i = 0; i < n; i++){
    	for(int j = 0; j < n; j++){
    		matrix[i][j] = tmp[i][j] = cnt;
    		cnt++;
		}
	}
	
	int x[T];
	pthread_t thread[T];
	for(int i=0;i<T;i++){
	    x[i] = i;
	    pthread_create(&thread[i], NULL, thread_function, &x[i]);
	}
	for(int i=0;i<T;i++){
	    pthread_join(thread[i], NULL);
	}
	
	//print the matrix
	/*
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}*/
	for(int i = 0; i < n; i++){
    	
    		free(matrix[i]);
		
	}
    return 0;
}
