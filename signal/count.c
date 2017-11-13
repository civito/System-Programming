#include <stdio.h>
#include <time.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdint.h> 

#define BUFFSIZE 50
#define BILLION 1000000000L


/*
static void sig_alrm(int signo) {}

unsigned int kill_sleep(unsigned int seconds, pid_t pid) {
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) 
		return(seconds);
	alarm(seconds);
        kill((int)pid, SIGALRM);
	pause();
	return(alarm(0)); 
}

unsigned int sleep1(unsigned int seconds) {
        if (signal(SIGALRM, sig_alrm) == SIG_ERR)
                return(seconds);
        alarm(seconds);
        pause();
        return(alarm(0));
}*/

void Print_time(struct timespec *start){
	struct timespec end;
	uint64_t diff;

	clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec-start->tv_sec) + end.tv_nsec-start->tv_nsec;
	printf("** time = %llu nsec\n", (long long unsigned int) diff);
	
}

int WrRd(char* file, pid_t pid, int num){
	int fd;
	int count=0;
        char rbuf[10];
        char wbuf[10];
	int i;

	fd = open(file, O_RDWR | O_SYNC, S_IRWXU | S_IRWXG | S_IRWXO );

	for(i=0; i<10000000; i++);

        if(read(fd, rbuf, 50)== -1){
        	printf("read Error!\n");
                return 0;
        }

        count = atoi(rbuf);
        count+=1;
        printf("P%d : %d, %d\n",num, pid, count);

        sprintf(wbuf, "%d", count);
        lseek(fd,0,SEEK_SET);
        write(fd,wbuf, sizeof(wbuf));

	close(fd);

	return count;
}


void myfunc(int signo){}

int main(int argc, char * argv[]){
	int fd;
	struct timespec start;
	int count=0;
	int num;
	pid_t pid;

	if(argc < 3){
		printf("ERROR - ./count (count num) (filename)\n");
		return 0;
	}

	if(atoi(argv[1]) < 1) {
		printf("ERROR");
		return 0;
	}

	fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO );

	if((write(fd, "0",1) != 1))
		printf("write error\n");

	close(fd);
	
	clock_gettime(CLOCK_MONOTONIC, &start); 

	
	if((pid = fork()) == 0){
       		 if((pid = fork()) == 0){
			signal(SIGUSR1, myfunc);
				pause();
//				sleep1(100);
			while(1){
				count = WrRd(argv[2], getpid(), 3);
                                if(count == atoi(argv[1])){
				       Print_time(&start);
                                       kill(getppid(),9);
                                       kill(getpid()-2,9);
                                       return 0;
                                }
//				sleep1(1);
			        kill((int)getpid()-2, SIGUSR1);
			        pause();
//				kill_sleep(100, getppid()-1);
			}
		}else if(pid > 0){
			signal(SIGUSR1, myfunc);
				pause();
//sleep1(100);
			while(1){
                                count = WrRd(argv[2], getpid(), 2);
                                if(count == atoi(argv[1])){
                                        Print_time(&start);
					kill(getppid(),9);
                                        kill(pid,9);
                                        return 0;
                                }
//	     	 		sleep1(1);
                                kill((int)pid, SIGUSR1);
                                pause();				
			}
		}else{
			printf("fork2() Error!\n");
		}		
		
	}else if(pid > 0){

		while(1){
			signal(SIGUSR1, myfunc);
                        count = WrRd(argv[2], getpid(), 1);
                        if(count == atoi(argv[1])){
	      		        Print_time(&start);
                                kill(pid,9);
                                kill(pid+1,9);
                                return 0;
                        }
//			sleep1(1);
//			kill_sleep(100, pid);
                        kill((int)pid, SIGUSR1);
                        pause();
		}
	}else{
		printf("fork() Error!\n");
	}

}
