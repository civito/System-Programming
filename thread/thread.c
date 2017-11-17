#include<stdio.h>
#include<time.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>

#define ARR_SIZE 4000
#define BILLION 1000000000L

/* Matrix multiplication C=A*B */
int** multi (int **A, int **B, int **C){
    int i,j,z;
    int sum = 0;

    for(z=0; z<ARR_SIZE; z++){
       for(i=0; i<ARR_SIZE; i++){
          for(j=0; j<ARR_SIZE; j++){
             sum += A[z][j]*B[j][i];
          }
          C[z][i] = sum;
          sum =0;
       }
    }
   
    return C;
}


int main(){
    struct timespec start,end;
    uint64_t diff,sum = 0;

/*  initializing MAtrix A, B  */

    FILE *fp = fopen("file1", "r");
    FILE *fp2 = fopen("file2", "r");

    int **A = (int**)malloc(sizeof(int*)*ARR_SIZE);
    int **B = (int**)malloc(sizeof(int*)*ARR_SIZE);
    int **C = (int**)malloc(sizeof(int*)*ARR_SIZE);

    int i,j,numA, numB;

    for(int i=0; i<ARR_SIZE; i++){
        A[i] = (int*)malloc(sizeof(int)*ARR_SIZE);
        B[i] = (int*)malloc(sizeof(int)*ARR_SIZE);
        C[i] = (int*)malloc(sizeof(int)*ARR_SIZE);
    }

    for(i=0; i<ARR_SIZE; i++){
        for(j=0; j<ARR_SIZE; j++){
             fscanf(fp, "%d", &numA);
             A[i][j] = numA;
             fscanf(fp2, "%d", &numB);
             B[i][j] = numB;
        }
    }
    
/* time checking Matrix multiplication & calcultaing sum */

    clock_gettime(CLOCK_MONOTONIC, &start);

    C = multi(A,B,C);
    
    for(i=0; i<ARR_SIZE; i++){
        for(j=0; j<ARR_SIZE; j++){
            sum += C[i][j];
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = BILLION * (end.tv_sec-start.tv_sec) + end.tv_nsec-start.tv_nsec;
    printf("** time = %llu msec\n", (long long unsigned int) diff / 1000000);
    

    return 0;
}
