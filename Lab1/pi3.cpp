#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int n;
int thread_num, flag = 0;
double sum = 0.0;
pthread_mutex_t mutex;

void *thread_sum(void *arg){
  int rank = *((int *)arg);
  int length = n / thread_num;
  int start = rank * length;
  int end = start + length;
  double factor, local_sum = 0.0;

  if(start % 2 == 0) factor = 1.0;
  else factor = -1.0;

  for(int i = start; i < end; i++, factor = -factor){
    local_sum = local_sum + factor / (2.0 * i + 1.0);
  }

  pthread_mutex_lock(&mutex);
  sum+=local_sum;
  pthread_mutex_unlock(&mutex);
  
  return NULL;
}

int main(int argc, const char * argv[]) {
    n = atoi(argv[1]);
	thread_num = atoi(argv[2]);
	//n=1e6; thread_num=8;
    pthread_t thread[thread_num];
    int x[thread_num];
	for(int i=0;i<thread_num;i++){  //Multithreading to break the problem
	    x[i] = i;
	    pthread_create(&thread[i], NULL, thread_sum, &x[i]);
	}
	for(int i=0;i<thread_num;i++){
	    pthread_join(thread[i], NULL);
	}
    //cout<<pi<<endl;
    printf("pi = %.12lf\n", 4.0*sum);
    return 0; 
} 
