#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
 
#define KEY_NUM 3332
#define MEM_SIZE 1024

struct temp{
    int k;
}

int main(){
     int shm_id;
     char buf_in[MEM_SIZE];
     void *shm_addr;
     
     while(-1 == (shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, 0666)));

     printf("Ready to attach!\n");
     
     if((void*)-1 == (shm_addr =shmat(shm_id, (void *) 0, 0))){
         printf("shmat fail!\n");
         return -1;
     }
//     char* call_num = (char*)shm_addr;

//     while(1){ 
     
     fgets(buf_in, MEM_SIZE, stdin );
     sprintf((char*)shm_addr, "%s", buf_in); 

     fgets(buf_in, MEM_SIZE, stdin );
     sprintf((char*)shm_addr, "%s", buf_in); 
         //sprintf((char*)shm_addr, "%s", buf_in);
//     }
}


