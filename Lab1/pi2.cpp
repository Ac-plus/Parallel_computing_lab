#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

int n, m, thread_num;
pthread_mutex_t mutex;

void *thread_sum(void *arg)
{
  int i, my_m = 0;
  int length = n / thread_num;
  float x, y;
  pthread_mutex_lock(&mutex);
  unsigned int seed = (unsigned int)(time(NULL) % 10000);
  pthread_mutex_unlock(&mutex);
  
  for(i=0; i<length; i++){
    x = rand_r(&seed) / (RAND_MAX*1.0);
    y = rand_r(&seed) / (RAND_MAX*1.0);
    if(x * x + y * y <= 1.0)
      my_m++;
  }

  pthread_mutex_lock(&mutex);
  m += my_m;
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char *argv[])
{
  int i;
  double pi;
  struct timespec time_start = {0, 0}, time_end = {0, 0};
  m = 0;
  n = atoi(argv[1]);
  thread_num = atoi(argv[2]);  //1st:blocks; 2nd:threads
  //n=100000; thread_num=4;
  
  pthread_mutex_init(&mutex, NULL);
  pthread_t *pt = (pthread_t *)malloc(sizeof(pthread_t) * thread_num);
  int *rank = (int *)malloc(sizeof(int) * thread_num);

  clock_gettime(CLOCK_REALTIME, &time_start);
  
  for(i = 0; i < thread_num; i++){
    rank[i] = i;
    if(pthread_create(&pt[i], NULL, thread_sum, (void *)&rank[i]) != 0){
      pthread_mutex_destroy(&mutex);
      free(pt);
      return 1;
    }
  }

  for(i = 0; i < thread_num; i++){
    pthread_join(pt[i], NULL);
  }

  pi = (4 * m * 1.0) / (n * 1.0);
  
  clock_gettime(CLOCK_REALTIME, &time_end);
  printf("pi = %.12lf\n", pi);
  pthread_mutex_destroy(&mutex);
  free(pt);
  return 0;
}
