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

    double rate = 0;
    for(int i = 0; i < ntimes; i++){
        rate += parallelDispatcher(type, loop, duration, fractionAdd, fractionRemove, 
                                   hitRate, maxBucketSize, mean, initSize, capacity, 
                                   nthreads);
        garbageCollect();
    }

    fprintf(output, "%lf\n", rate /ntimes);

    return 0;
}





