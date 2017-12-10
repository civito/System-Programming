#include "hybrid_lock.h"
#include<stdint.h>

void hybrid_lock_init(int count)
{
    hlock.wait_thr = 0;
    hlock.Lcount = count;
    pthread_mutex_init(&hlock.mutex, NULL);
    pthread_spin_init(&hlock.spin, NULL);
}
 
void hybrid_lock_destroy()
{
    pthread_mutex_destroy(&hlock.mutex);
    pthread_spin_destroy(&hlock.spin);
}
 
void hybrid_lock_lock()
{
    struct timeval START, END;
    int time;
    int count = 0;
    uint64_t diff =0;

    printf("******%d\n", hlock.Lcount);

//    gettimeofday(&START,NULL);
    while(pthread_spin_trylock(&hlock.spin) != 0){
        count++;
        if(count == hlock.spin){
           break;
        }
    }
//    gettimeofday(&END,NULL);
//    diff = END.tv_usec-START.tv_usec;
//    printf("** Sum time = %llu \n", (long long unsigned int) diff);
    hlock.wait_thr += 1;
    pthread_mutex_lock(&hlock.mutex);
    hlock.wait_thr -= 1;
}
    
void hybrid_lock_unlock()
{
    if(hlock.wait_thr > 0){        
        pthread_mutex_unlock(&hlock.mutex);
    }else{
        pthread_spin_unlock(&hlock.spin);
        pthread_mutex_unlock(&hlock.mutex);
    }
}

