#include <signal.h>
#include "apue.h"
#include <stdio.h>


void sig_fn(int);

int main(){
	static struct sigaction act;
	act.sa_handler=sig_fn;
	
	sigfillset(&(act.sa_mask));
	sigaction(SIGINT,&act,NULL);
	for(;;)
		pause();
		
}

void sig_fn(int signo){
	printf("\n");
	if(signo==SIGINT)
		printf("CTRL-C is pressed. Try Again");
	
}
