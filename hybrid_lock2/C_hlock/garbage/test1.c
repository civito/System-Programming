#include "hybrid_lock.h"
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#define TIME 1000000

long g_count = 0;

pthread_spinlock_t t_spin;

void *test_count(void *arg){
	int count = 0;
	int Lcount = 0;
	struct timeval start, end;
	uint64_t diff =0;

	gettimeofday(&start,NULL);
	while(pthread_spin_trylock(&t_spin) != 0){
        	count+= 1;
		if(count == 1000) break;
	}
	gettimeofday(&end,NULL);
	diff = end.tv_usec-start.tv_usec;
	printf("** Sum time = %llu \n", (long long unsigned int) diff); 
	Lcount = 1000000000 / diff;

	printf("** Lcount = %d \n", Lcount); 
    return ((void *)Lcount);
}


void *thread_func(void *arg)
{
	long i, count = (long)arg;

	hybrid_lock_lock();
	for (i = 0; i<count; i++) {
		g_count++;
	}
	hybrid_lock_unlock();
}

int main(int argc, char *argv[])
{
	pthread_t *tid;
	pthread_t *t_tid;

	long i, thread_count, value;
	int rc;
    int Lcount;
    struct timeval mstart, mend;
    uint64_t diff =0;

	if (3 != argc) {
		fprintf(stderr, "usage: %s <thread count> <value>\n", argv[0]);
		exit(0);
	}


	thread_count = atol(argv[1]);
	value = atol(argv[2]);

	pthread_spin_init(&t_spin,NULL);

	pthread_spin_lock(&t_spin);

	rc = pthread_create(&t_tid, NULL, test_count, NULL);
	if (rc) {
		fprintf(stderr, "pthread_create() error\n");
		free(tid);
		exit(0);
	}

	pthread_join(t_tid, &Lcount);
	pthread_spin_unlock(&t_spin);
	pthread_spin_destroy(&t_spin);

	hybrid_lock_init(Lcount);

	tid = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);
	if (!tid) {
		fprintf(stderr, "malloc() error\n");
		exit(0);
	}

    gettimeofday(&mstart,NULL);
	for (i = 0; i<thread_count; i++) {
		rc = pthread_create(&tid[i], NULL, thread_func, (void*)value);
		if (rc) {
			fprintf(stderr, "pthread_create() error\n");
			free(tid);

			exit(0);
		}
	}

	for (i = 0; i<thread_count; i++) {
		rc = pthread_join(tid[i], NULL);
		if (rc) {
			fprintf(stderr, "pthread_join() error\n");
			free(tid);

			exit(0);
		}
	}
    gettimeofday(&mend,NULL);
    diff = TIME * (mend.tv_sec-mstart.tv_sec) + mend.tv_usec-mstart.tv_usec;
    printf("** Sum time = %llu.%llu sec\n", (long long unsigned int) diff / 1000000, ((long long unsigned int) diff/1000));


	printf("value: %ld\n", g_count);

    hybrid_lock_destroy();
	free(tid);
}
