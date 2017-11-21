#include<stdio.h>
#include<time.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<pthread.h>

#define T_NUM 40 // Number of Thread 

#define ARR_SIZE 4000
#define TIME 1000000

int **A;
int **B;
uint64_t **C;

uint64_t SUM[ARR_SIZE] = {0,};

pthread_barrier_t bar;

/* Matrix multiplication C=A*B */
void multi (int from, int to, int t_num){
    int i,j,z;
    uint64_t sum = 0;

    for(z=from; z<to; z++){
       for(i=0; i<ARR_SIZE; i++){
          for(j=0; j<ARR_SIZE; j++){
             SUM[t_num] += A[z][j]*B[j][i];
             sum += A[z][j]*B[j][i];
          }
          C[z][i] = sum;
          sum =0;
       }
    }
    pthread_barrier_wait(&bar);
}

/* Thread Function*/ 
void * thr_fn(void * arg){
    int num = ARR_SIZE / T_NUM;

    if((long)arg == (T_NUM-1))
        multi(num*(long)arg, ARR_SIZE, (long)arg);
    else
        multi(num*(long)arg, num*((long)arg+1), (long)arg);
}


int main(){
    struct timeval START, END;
    pthread_t tid[T_NUM];
    uint64_t diff, sum2 = 0;
    uint64_t sum = 0;
    pthread_barrier_init(&bar,NULL,T_NUM);

/*  initializing MAtrix A, B  */

    FILE *fp = fopen("sample1.txt", "r");
    FILE *fp2 = fopen("sample2.txt", "r");

    A = (int**)malloc(sizeof(int*)*ARR_SIZE);
    B = (int**)malloc(sizeof(int*)*ARR_SIZE);
    C = (uint64_t**)malloc(sizeof(uint64_t*)*ARR_SIZE);

    int j,numA, numB;
    long i;

    for(int i=0; i<ARR_SIZE; i++){
        A[i] = (int*)malloc(sizeof(int)*ARR_SIZE);
        B[i] = (int*)malloc(sizeof(int)*ARR_SIZE);
        C[i] = (uint64_t*)malloc(sizeof(uint64_t)*ARR_SIZE);
    }

    for(i=0; i<ARR_SIZE; i++){
        for(j=0; j<ARR_SIZE; j++){
             fscanf(fp, "%d\t", &numA);
             A[i][j] = numA;
             fscanf(fp2, "%d\t", &numB);
             B[i][j] = numB;
        }
    }


    
/* time checking Matrix multiplication & calcultaing sum */

    printf("%d Thread is running ...\n",T_NUM);

    gettimeofday(&START,NULL);

    /* Thread = T_NUM */
    for(i=0; i<T_NUM; i++){
       pthread_create(&tid[i], NULL, thr_fn, (void *)i);  
    }

    for(i=0; i<T_NUM; i++){               
        pthread_join(tid[i], NULL);
    }      

    for(i=0; i<ARR_SIZE; i++){
        for(j=0; j<ARR_SIZE; j++){
            sum += C[i][j];
       }
    }

    for(i=0; i<T_NUM; i++)
        sum2 += SUM[i];

    gettimeofday(&END,NULL);
    
    printf("** sum = %llu\n  sum2 = %llu\n", sum,sum2);
    diff = TIME * (END.tv_sec-START.tv_sec) + END.tv_usec-START.tv_usec;
    printf("** time = %llu.%llu sec\n", (long long unsigned int) diff / 1000000, ((long long unsigned int) diff/1000)%1000);
    pthread_barrier_destroy(&bar);
    

    return 0;
}
