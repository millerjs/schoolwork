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


int experiment1(FILE *output)
{
    
    int nPackets = 0;
    double elapsedTime = 0;

    hasht_type_t type    = LOCKING;
    loop_t loop          = no_load_loop;
    int duration         = 2000;
    float fractionAdd    = .9;
    float fractionRemove = .1;
    float hitRate        = .5;
    int maxBucketSize    = 32;
    long mean            = 1;
    int initSize         = 10;
    int capacity         = 32;
    int nthreads         = 1;

    int ntimes = 10;

    for(int i = 0; i < ntimes; i++){
        parallelDispatcher(type, loop, duration, 
                           fractionAdd, fractionRemove, 
                           hitRate, maxBucketSize, mean, 
                           initSize, capacity, nthreads, 
                           &nPackets, &elapsedTime);
        garbageCollect();

    }
    
    fprintf(output, "Rate : %lf\n", nPackets / elapsedTime);

    return 0;
}





