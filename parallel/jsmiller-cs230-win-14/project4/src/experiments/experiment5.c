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

int experiment5(FILE *output, hasht_type_t type, 
                float fractionAdd, float fractionRemove, 
                float hitRate, int nthreads)
{
    int ntimes = 10;

    int capacity         = 32;
    int duration         = 2000;
    loop_t loop          = worker_loop;
    int maxBucketSize    = 32;
    long mean            = 4000;
    int initSize         = 1;

    double rate = 0;
    for(int i = 0; i < ntimes; i++){
        rate += parallelDispatcher(type, loop, duration, 
                                   fractionAdd, fractionRemove, 
                                   hitRate, maxBucketSize, mean, 
                                   initSize, capacity, nthreads);
        garbageCollect();
    }

    fprintf(output, "%lf \t %d \t %d\n", 
            rate/ntimes, 
            capacity, 
            OPTIMISM);

    fflush(output);

    return 0;
}





