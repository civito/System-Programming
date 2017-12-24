#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/msg.h>

#define KEY_NUM 3228 // Shared Memory
#define MSQ_KEY_NUM 3333 // Msg Queue

#define SEND 1
#define RCV 2
 
#define MEM_SIZE 1024
#define LEN 10


typedef struct{
	long mtype;
	int to;
	char mtext[10];
}MsgType;

typedef struct{
        int PID;
        char msg_text[10];
}ChatLog;

int main(){
        int log_cnt = 0;
        ChatLog * chatlog;
        int *cnt;
        void*shared_memory = (void*)0;
        shmdt((void *)chatlog);

	key_t key = MSQ_KEY_NUM;
	int que_id = msgget(key, IPC_CREAT|0666);
	MsgType msg;
	int msg_size = sizeof(msg) - sizeof(msg.mtype);
	int option;
	int contin;
	char temp;
	char buf_text[1024];


        int shm_id;
        char buf_in[MEM_SIZE];
        int size = sizeof(ChatLog) * LEN;

        if(-1 == (shm_id = shmget((key_t)KEY_NUM ,size, IPC_CREAT|0666))){
                printf("shmget fail!\n");
                return -1;
        }

        chatlog = shmat(shm_id, (void *)0, SHM_RDONLY);

//        shmctl(shm_id, IPC_RMID, NULL);


	while(1){
		printf("=========================\n");
		printf("Select Option\n");
		printf("1. Message Send\n");
		printf("2. Message Receive\n");
		printf("3. Confirm Message_log\n");
		printf("4. Exit\n");
		printf("==========================\n");
		scanf("%d", &option);
		printf("\n\n");

		switch(option){
			case 1 : // mqsnd
				system("clear");
				printf("To who?\n");
				scanf("%d", &msg.to);
				printf("Write your Message : \n");
				scanf("%s", buf_text);
				msg.mtype = SEND;				
				strcpy(msg.mtext, buf_text);
				msgsnd(que_id, &msg, msg_size, NULL); 
				system("clear");
			break;

                        case 2 : // mqrcv				
				while(msgrcv(que_id, &msg, msg_size, RCV, IPC_NOWAIT) != (ssize_t)-1){
					printf("%s\n", msg.mtext);			
				}
				printf("\n");
			break;

                        case 3 : // access shared memory
				printf("========= Message Log ========\n\n");
				while(log_cnt < 100){
					if(chatlog[log_cnt].PID != 0){
					printf("[%d] : %s\n", chatlog[log_cnt].PID, chatlog[log_cnt].msg_text);}
					log_cnt++;
				}
				log_cnt = 0;
				printf("\n\n");
			break;

			case 4 :
				shmdt(chatlog);
				return 0;


			default:
				printf("Undefined function\n");
			break;
		}
	}
}


