/******************************************************************************
 *
 * FILE    : hasht_handler.c                              
 * DESCRIP : header file for threads
 * AUTHOR  : Joshua Miller
 * PROJECT : Project 3
 * CLASS   : Parallel Computing - Winter 2014
 *
 ******************************************************************************/

#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "../thread_pool.h"
#include "../util.h"
#include "../cll.h"
#include "../hasht_handler.h"

#include "../libhasht/hasht.h"

extern volatile int OPTIMISM;

int experiment4(FILE *output)
{
    int ntimes = 10;

    int capacity         = 32;
    int duration         = 2000;
    hasht_type_t type    = AWESOME;
    loop_t loop          = worker_loop;
    float fractionAdd    = .45;
    float fractionRemove = .05;
    float hitRate        = .9;
    int maxBucketSize    = 32;
    long mean            = 4000;
    int initSize         = 1;
    /* int nthreads         = 1; */
    
    for(int n = 1; n < 5; n++){
        
        int nthreads = (1<<n) - 1;

        for (capacity = 1; capacity <= 512; capacity *= 2){ 
            for(OPTIMISM = 1; OPTIMISM <= capacity; OPTIMISM *= 2){

                double rate = 0;
                for(int i = 0; i < ntimes; i++){
                    rate += parallelDispatcher(type, loop, duration, 
                                               fractionAdd, fractionRemove, 
                                               hitRate, maxBucketSize, mean, 
                                               initSize, capacity, nthreads);
                    garbageCollect();
                }

                fprintf(output, "%lf \t %d \t %d \t %dn\n", 
                        rate/ntimes, 
                        capacity, 
                        OPTIMISM, 
                        nthreads);

                fprintf(stderr, "%lf \t %d \t %d\n", 
                        rate/ntimes, 
                        capacity, 
                        OPTIMISM);

                fflush(output);
                fflush(stderr);

            }
        }
    }


    return 0;
}





