#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

struct hybrid_lock {
    pthread_mutex_t mutex;
    pthread_spinlock_t spin;
    int wait_thr;
}hlock;


void hybrid_lock_init();
void hybrid_lock_destroy();
void hybrid_lock_lock();
void hybrid_lock_unlock();
