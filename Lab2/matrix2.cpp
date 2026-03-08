#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int **matrix;
int **tmp;
int T, n;

void Swp_itm(int &x, int &y) {  //swap
	int tmp=x;
	x=y;
	y=tmp;
} 

void *thread_func(void *arg){
    int begin = *(int *)arg;
    //int end = (begin == T-1) ? n : begin + n/T;
    int end = (begin == T-1) ? n : sqrt(begin+1)* n/sqrt(T);
    begin = sqrt(begin)* n/sqrt(T); 
    //begin = begin * (sqrt(begin+1)-sqrt(begin))*n/sqrt(T);
    
    for(int i=begin; i<end; i++){
    	for(int j=0; j<i; j++){
    		Swp_itm(matrix[i][j], matrix[j][i]);
    		//matrix[i][j]=0; matrix[j][i]=0;
	        //int tmp = matrix[i][j];
	    	//matrix[i][j] = matrix[j][i];
	        //matrix[j][i] = tmp;
    	}
	}
    return NULL;
}

int main(int argc, const char* argv[]) {
    n = atoi(argv[1]);
	T = atoi(argv[2]);
	
	//init the matrix
	matrix = (int **)malloc(n * sizeof(int *));
    tmp = (int **)malloc(n * sizeof(int *));
    for(int i=0; i<n; i++){
        matrix[i] = (int *)malloc(n * sizeof(int));
        tmp[i] = (int *)malloc(n * sizeof(int));
    }
    int cnt = 0;
    for(int i=0; i<n; i++){
    	for(int j=0; j<n; j++){
    		matrix[i][j] = tmp[i][j] = cnt;
    		cnt++; 
		}
	}

	int x[T];
	pthread_t thread[T];
	for(int i=0;i<T;i++){
	    x[i] = i;
	    pthread_create(&thread[i], NULL, thread_func, &x[i]);
	}
	for(int i=0;i<T;i++){
	    pthread_join(thread[i], NULL);
	}
	
	/*
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	*/
	for(int i = 0; i < n; i++){
    	free(matrix[i]);
	}
    return 0;
}
