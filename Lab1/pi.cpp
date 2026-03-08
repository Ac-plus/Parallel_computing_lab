#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "unistd.h"
using namespace std;

float pi = 0.0;
long long N;
int T;
pthread_mutex_t mut;
void *thread_function1(void *arg){
    int id = *(int *)arg;
    int piece = N/T;
    int start = id*piece;
    int end = start + piece;
    double local_pi = 0.0;
    for(int i=start;i<end;i++){
        local_pi += (4.0/(1.0+((i+0.5)/N)* ((i+0.5)/N)))/N;
    }
    pthread_mutex_lock(&mut);
    pi += local_pi;
    pthread_mutex_unlock(&mut);
    return NULL;
}
int main(int argc, const char * argv[]) {
    N = atoi(argv[1]);
	T = atoi(argv[2]);
	//N=1e5; T=8;
    pthread_t thread[T];
    int x[T];
	for(int i=0;i<T;i++){  //Multithreading to break the problem
	    x[i] = i;
	    pthread_create(&thread[i], NULL, thread_function1, &x[i]);
	}
	for(int i=0;i<T;i++){
	    pthread_join(thread[i], NULL);
	}
    //cout<<pi<<endl;
    printf("pi = %.12lf\n", pi);
    return 0; 
} 
