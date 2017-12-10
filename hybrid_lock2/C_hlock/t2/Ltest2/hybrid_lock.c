#include "hybrid_lock.h"
#include<stdint.h>
#define TIME 1000000

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
    struct timeval START2, END2;
    long long time;
    long long count = 0;
    uint64_t diff =0;
    uint64_t diff2 =0;
    long long int i = 0;
    long long int j = 0;

//    printf("******%d\n", hlock.Lcount);


////////////////////////////////////////////////////////////////

    pthread_spin_lock(&hlock.spin);


    gettimeofday(&START,NULL);
    while(pthread_spin_trylock(&hlock.spin) != 0){
//         printf("I'm spinning!\n");
	 i++;
//         gettimeofday(&END,NULL);
         if(i == 1000){
            break;
         }
    }
    gettimeofday(&END,NULL);
    diff = TIME *(END.tv_sec-START.tv_sec) + END.tv_usec-START.tv_usec;
    hlock.Lcount = 1000000000 / diff;
    printf("Lcount = %d\n", hlock.Lcount);


//////////////////////////////////////////////////////////////////////////////////


    gettimeofday(&START2,NULL);
    gettimeofday(&START,NULL);
    while(pthread_spin_trylock(&hlock.spin) != 0){
        count++;
        if(count == hlock.Lcount*6){
	   gettimeofday(&END,NULL);
	   if((TIME * (END.tv_sec-START.tv_sec) + END.tv_usec-START.tv_usec) < TIME){
		j ++;
		hlock.Lcount += 100000;
                continue;
	   }                
           break;
        }
    }
    gettimeofday(&END2,NULL);
    printf(" j = %lld\n", j);
    printf("count = %lld\n", count);
    
    diff2 = 1000000 * (END.tv_sec-START.tv_sec) + END.tv_usec-START.tv_usec;
 printf("** time = %llu usec\n", (long long unsigned int) diff2);

    diff = 1000000 * (END2.tv_sec-START2.tv_sec) + END2.tv_usec-START2.tv_usec;
    printf("** time = %llu usec\n", (long long unsigned int) diff);
//////////////////////////////////////////////////////////////////

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

