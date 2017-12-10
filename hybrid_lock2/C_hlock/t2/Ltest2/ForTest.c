#include "hybrid_lock.h"
#include <stdlib.h>
#include <pthread.h>
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
	long i, j, k, count = (long)arg;
	long long l;

	/*
	 * Increase the global variable, g_count.
	 * This code should be protected by
	 * some locks, e.g. spin lock, and the lock that 
	 * you implemented for assignment,
	 * because g_count is shared by other threads.
	 */

    hybrid_lock_lock();
//	pthread_mutex_lock(&hlock.mutex);
	for (i = 0; i<count; i++) {

		/********************** Critical Section **********************/

		/*
		 * The purpose of this code is to occupy cpu for long time.
		 */
		for (j = 0; j<100000; j++)
			for (k = 0; k<3000; k++)
				l += j * k;

		g_count++;
		/**************************************************************/
	}
//	pthread_mutex_unlock(&hlock.mutex);
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


	/*
	 * Check that the program has three arguments
	 * If the number of arguments is not 3, terminate the process.
	 */
	if (3 != argc) {
		fprintf(stderr, "usage: %s <thread count> <value>\n", argv[0]);
		exit(0);
	}

	/*
	 * Get the values of the each arguments as a long type.
	 * It is better that use long type instead of int type,
	 * because sizeof(long) is same with sizeof(void*).
	 * It will be better when we pass argument to thread
	 * that will be created by this thread.
	 */
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

	/*
	 * Create array to get tids of each threads that will
	 * be created by this thread.
	 */
	tid = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);
	if (!tid) {
		fprintf(stderr, "malloc() error\n");
		exit(0);
	}

	hybrid_lock_init(Lcount);

	/*
	 * Create a threads by the thread_count value received as
	 * an argument. Each threads will increase g_count for
	 * value times.
	 */
    gettimeofday(&mstart,NULL);
	for (i = 0; i<thread_count; i++) {
		rc = pthread_create(&tid[i], NULL, thread_func, (void*)value);
		if (rc) {
			fprintf(stderr, "pthread_create() error\n");
			free(tid);
//			pthread_mutex_destroy(&g_mutex);
			exit(0);
		}
	}

	/*
	 * Wait until all the threads you created above are finished.
	 */

	for (i = 0; i<thread_count; i++) {
		rc = pthread_join(tid[i], NULL);
		if (rc) {
			fprintf(stderr, "pthread_join() error\n");
			free(tid);
//			pthread_mutex_destroy(&g_mutex);
			exit(0);
		}
	}
    gettimeofday(&mend,NULL);
    diff = TIME * (mend.tv_sec-mstart.tv_sec) + mend.tv_usec-mstart.tv_usec;
    printf("** Sum time = %llu.%llu sec\n", (long long unsigned int) diff / 1000000, ((long long unsigned int) diff/1000));

    hybrid_lock_destroy();

	/*
	 * Print the value of g_count.
	 * It must be (thread_count * value)
	 */ 
	printf("value: %ld\n", g_count);

	free(tid);
}