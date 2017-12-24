#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define KEY_NUM 3228

#define MSQ1_KEY_NUM 1111
#define MSQ2_KEY_NUM 2222
#define MSQ3_KEY_NUM 3333

#define SEND 2
#define RCV 1

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
	
	ChatLog *chatlog;
	int *cnt;
	void*shared_memory = (void*)0;
 
        key_t key1 = MSQ1_KEY_NUM;
        key_t key2 = MSQ2_KEY_NUM;
        key_t key3 = MSQ3_KEY_NUM;

        int que_id1 = msgget(key1, IPC_CREAT|0666);
        int que_id2 = msgget(key2, IPC_CREAT|0666);
	int que_id3 = msgget(key3, IPC_CREAT|0666);

	MsgType msg;
	int msg_size = sizeof(msg) - sizeof(msg.mtype);
	char buf_text[1024];


        int shm_id;
        char buf_in[MEM_SIZE];
	int size = sizeof(ChatLog) * LEN;


        if(-1 == (shm_id = shmget((key_t)KEY_NUM , size , IPC_CREAT|0666))){
                printf("shmget fail!\n");
                return -1;
        }

        if((void*)-1 == (chatlog = (ChatLog *)shmat(shm_id, (void *)0, 0))){
                printf("shmat fail!\n");
                return -1;
        }
//	shmctl(shm_id, IPC_RMID, NULL);	

	while(1){

		if(msgrcv(que_id1, &msg, msg_size, RCV, IPC_NOWAIT) != (ssize_t)-1){
			/* when target process exist */
			if(msg.to == 2){
				msg.mtype = SEND;	
				msgsnd(que_id2, &msg, msg_size, IPC_NOWAIT);
			}else if(msg.to == 3){
                                msg.mtype = SEND;
                                msgsnd(que_id3, &msg, msg_size, IPC_NOWAIT);
			}else{
			/* Just write on log */	
				chatlog[log_cnt].PID = 1;
				strcpy(chatlog[log_cnt].msg_text, msg.mtext);
				log_cnt++;
				printf("%s\n", msg.mtext);
			}


                }else if(msgrcv(que_id2, &msg, msg_size, RCV, IPC_NOWAIT) != (ssize_t)-1){
                        /* when target process exist */
                        if(msg.to == 1){        
                                msg.mtype = SEND;
                                msgsnd(que_id1, &msg, msg_size, IPC_NOWAIT);
                        }else if(msg.to == 3){
                                msg.mtype = SEND;
                                msgsnd(que_id3, &msg, msg_size, IPC_NOWAIT);
                        }else{
                        /* Just write on log */ 
                                printf("%s\n", msg.mtext);
                        }
                }else if(msgrcv(que_id3, &msg, msg_size, RCV, IPC_NOWAIT) != (ssize_t)-1){
                        /* when target process exist */
                        if(msg.to == 1){        
                                msg.mtype = SEND;
                                msgsnd(que_id1, &msg, msg_size, IPC_NOWAIT);
                        }else if(msg.to == 2){
                                msg.mtype = SEND;
                                msgsnd(que_id2, &msg, msg_size, IPC_NOWAIT);
                        }else{
                        /* Just write on log */ 
                                printf("%s\n", msg.mtext);
                        }

                }else;
	}

}

