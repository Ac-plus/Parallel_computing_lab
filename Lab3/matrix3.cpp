#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>
#include<time.h>

int blocksize;
typedef struct subblock {
  int *vl;
}block;

void trans(void *arg)
{
    int tmp, index1, index2;
    int *vl = (int *)arg;
    for(int i = 1; i < blocksize; i++){
        for(int j = 0; j < i; j++){
            index1 = i * blocksize + j;
            index2 = j * blocksize + i;
            tmp = vl[index1];
            vl[index1] = vl[index2];
            vl[index2] = tmp;
        }
    }
}

int main(int argc, char *argv[])
{   
    block **a;
    int *recv;
    int n, size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) {
        int i, j, k, m, r, *temp;
        MPI_Status status;
        unsigned int seed = (unsigned int)(time(NULL) % 10000);
        struct timespec time_start = {0, 0}, time_end = {0, 0};
        n = atoi(argv[1]);
        blocksize = (int)sqrt((n * n * 1.0) / size);
        m = n / blocksize;
        a = (block **)malloc(sizeof(block *) * m);
        for(i = 0; i < m; i++)
        {
            a[i] = (block *)malloc(sizeof(block) * m);
        }
        for(i = 0; i < m; i++)
        {
            for(j = 0; j < m; j++)
            {
                a[i][j].vl = (int *)malloc(sizeof(int) * blocksize * blocksize);
            }
        }
        for(i = 0; i < m; i++)
        {
            for(j = 0; j < m; j++)
            {
                for(k = 0; k < blocksize * blocksize; k++)
                {
                    a[i][j].vl[k] = rand_r(&seed) % 100;
                }
            }
        }
        clock_gettime(CLOCK_REALTIME, &time_start);
        for(i = 1; i < m; i++)
        {
            for(j = 0; j < i; j++)
            {
                temp = a[i][j].vl;
                a[i][j].vl = a[j][i].vl;
                a[j][i].vl = temp;
            }
        }
        for(i = 0; i < m; i++)
        {
            for (j = 0; j < m; j++)
            {
                r = i * m + j;
                if(r != 0)
                {
                    MPI_Send(a[i][j].vl, blocksize * blocksize, MPI_INT, r, r, MPI_COMM_WORLD);
                }
            }
        }
        trans(a[0][0].vl);       
        for(i = 0; i < m; i++)
        {
            for (j = 0; j < m; j++)
            {
                r = i * m + j;
                if(r != 0)
                {
                    MPI_Recv(a[i][j].vl, blocksize * blocksize, MPI_INT, r, r, MPI_COMM_WORLD, &status);
                }
            }
        }

        clock_gettime(CLOCK_REALTIME, &time_end);

        for(i = 0; i < m; i++)
        {
            for(j = 0; j < m; j++)
            {
                free(a[i][j].vl);
            }
            free(a[i]);
        }
        free(a);
    }
    else
    {
    	MPI_Status status;
    	n = atoi(argv[1]);
        blocksize = (int)sqrt((n * n * 1.0) / size);
        recv = (int *)malloc(sizeof(int) * blocksize * blocksize);
        MPI_Recv(recv, blocksize * blocksize, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
        trans(recv);      
        MPI_Send(recv, blocksize * blocksize, MPI_INT, 0, rank, MPI_COMM_WORLD);
        free(recv);
    }

    MPI_Finalize();
  
    return 0;
}