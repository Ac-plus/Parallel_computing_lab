#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int **matrix;

void trans(int row,int column,int n){  //从r行c列开始对n块数的矩阵做转置
                                       //使用开辟临时变量tmp然后赋值给matrix的方法实现
    for(int i=row;i<n;i++){
        for(int j=column;j<i;j++){
            int tmp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = tmp;
        }
    }
}

void init(int n){
    matrix = (int **)malloc(n * sizeof(int *));
    for(int i=0;i<n;i++){
        matrix[i] = (int *)malloc(n * sizeof(int));
    }
    int cnt = 0;
    for(int i=0;i<n;i++){
    	for(int j=0;j<n;j++){
    		matrix[i][j] = cnt;
    		cnt++;
		}
	}
}

int main(int argc, const char * argv[]) {
	int n = atoi(argv[1]);
	//init;
	matrix = (int **)malloc(n * sizeof(int *));
    for(int i=0;i<n;i++){
        matrix[i] = (int *)malloc(n * sizeof(int));
    }
    int cnt = 0;
    for(int i=0;i<n;i++){
    	for(int j=0;j<n;j++){
    		matrix[i][j] = cnt;
    		cnt++;
		}
	}


	trans(0,0,n);
	//print(n);
	/*
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	} */

	//free the matrix's memory space
	free(matrix);
			
    return 0;
}
