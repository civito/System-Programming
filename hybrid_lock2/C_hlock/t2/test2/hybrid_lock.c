#include "hybrid_lock.h"
#include<stdint.h>
#define TIME 1000000

void hybrid_lock_init()
{
    hlock.wait_thr = 0;
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
    struct timeval START2, END2;
    long time;
    int i=0;
    uint64_t diff =0;
/////////////////////////////////////////////////////////////////////////////

    pthread_spin_lock(&hlock.spin);

    gettimeofday(&START2,NULL);
    gettimeofday(&START,NULL);
    while(pthread_spin_trylock(&hlock.spin) != 0){
//         printf("I'm spinning!\n");
	 i++;
         gettimeofday(&END,NULL);
         if(time=(TIME *(END.tv_sec-START.tv_sec) + END.tv_usec-START.tv_usec)>=TIME){
//         if(time=(END.tv_sec-START.tv_sec) >= 1){
            break;
         }
    }
    gettimeofday(&END2,NULL);
     printf("time : %ld\n", time);
     printf("Number of spin : %d\n", i);
     diff = 1000000 * (END2.tv_sec-START2.tv_sec) + END2.tv_usec-START2.tv_usec;
    printf("** time = %llu.%llu sec\n", (long long unsigned int) diff / 1000000, ((long long unsigned int) diff/1000));

////////////////////////////////////////////////////////////////////////////

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

