#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/msg.h>

 
#define KEY_NUM 3332 // Shared Memory

#define MSQ_KEY_SND 1110 // Msg Queue
#define MSQ_KEY_RCV 1111
 
#define MEM_SIZE 1024

typedef struct{
	long mtype;
	int to;
	char mtext[1024];
}MsgType;


int main(){
	key_t skey = MSQ_KEY_SND;
	key_t rkey = MSQ_KEY_RCV;
	int snd_que = msgget(skey, IPC_CREAT|0666);
	int rcv_que = msgget(rkey, IPC_CREAT|0666); 
	MsgType msg;
	int msg_size = sizeof(msg) - sizeof(msg.mtype);
	int option;
	char buf_text[1024];

	while(1){
		printf("=========================\n");
		printf("Select Option\n");
		printf("1. Message Send\n");
		printf("2. Message Receive\n");
		printf("3. Confirm Message_log\n");
		printf("==========================\n");
		scanf("%d", &option);

		switch(option){
			case 1 : // mqsnd
				printf("To who?\n");
				scanf("%d", &msg.to);
				printf("Write your Message : \n");
				scanf("%s", buf_text);
				msg.mtype = 1;				
				strcpy(msg.mtext, buf_text);
				msgsnd(snd_que, &msg, msg_size, NULL); 
			break;

                        case 2 : // mqrcv
				
				while(msgrcv(rcv_que, &msg, msg_size, 0, IPC_NOWAIT) != (ssize_t)-1){		
					printf("%s\n", msg.mtext);			
				}
			break;

                        case 3 : // access shared memory

			break;

			default:
				printf("Undefined function\n");
			break;
		}
	}













/*
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
*/


}


