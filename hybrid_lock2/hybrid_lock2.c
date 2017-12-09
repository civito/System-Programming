#include "hybrid_lock.h"
#include<stdint.h>

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
    int Lcount = 0;
    struct timeval start, end;
    int count = 0;
    uint64_t diff =0;
  
    // First Measure
    gettimeofday(&start,NULL);
    while(pthread_spin_trylock(&hlock.spin) != 0){
        count+= 1;
        if(count == 10000) break;
    }
    printf("count = %d\n",count);
    gettimeofday(&end,NULL);
    diff = end.tv_usec-start.tv_usec;
    printf("****** Sum time = %llu \n", (long long unsigned int) diff);
    Lcount = 100000000000 / diff;
    printf("******* Lcount = %d \n", Lcount);


    count = 0;

    // Second Measure
    gettimeofday(&start,NULL);
    while(pthread_spin_trylock(&hlock.spin) != 0){
        count+= 1;
        if(count == 10000) break;
    }
    printf("count = %d\n",count);
    gettimeofday(&end,NULL);
    diff = end.tv_usec-start.tv_usec;
    printf("****** Sum time = %llu \n", (long long unsigned int) diff);
    Lcount = 100000000000 / diff;
    printf("******* Lcount = %d \n", Lcount);


/*    struct timeval START, END;
    int time;

    gettimeofday(&START,NULL);
    while(pthread_spin_trylock(&hlock.spin) != 0){
//         printf("I'm spinning!\n");
         gettimeofday(&END,NULL);
         if(time=(END.tv_sec -START.tv_sec)>=1){
            break;
         }
    }
    hlock.wait_thr += 1;
    pthread_mutex_lock(&hlock.mutex);
    hlock.wait_thr -= 1;*/

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

