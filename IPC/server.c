#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define KEY_NUM 3332
#define MEM_SIZE 1024

int main(){
    int shm_id;
    char buf_in[MEM_SIZE];
    void *shm_addr;
    
    if(-1 == (shm_id = shmget((key_t) KEY_NUM,MEM_SIZE, IPC_CREAT|0666))){
        printf("shmget fail!\n");
        return -1;
    }

    if((void*)-1 == (shm_addr =shmat(shm_id, (void *) 0, 0))){
        printf("shmat fail!\n");
        return -1;
    }
//    strcpy(buf_in, (char*)shm_addr);

    while(1){
//        if(call_num != NULL){
        printf("%s", (char *)shm_addr);
//        }
    }
  
    

}

