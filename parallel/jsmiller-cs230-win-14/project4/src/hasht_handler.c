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

#include "hasht_handler.h"

void parallelDispatcher(hasht_type_t type, 
                        loop_t loop, 
                        int nmillisec,
                        float fractionAdd,
                        float fractionRemove,
                        float hitRate,
                        int maxBucketSize,
                        long mean,
                        int initSize,
                        int nthreads)
{
    StopWatch_t timer;
	
    /* 
     * HashPacketGenerator_t * source = 
     *     createHashPacketGenerator(fractionAdd,fractionRemove,hitRate,mean);
     * 
     * thread_pool_t *pool = thread_pool_create(loop, nthreads);
     * pool->table = hasht_new(type, initSize, nthreads);
     */

    //
    // initialize your hash table w/ initSize number of add() calls using
    // getAddPacket();
    //
    // allocate and initialize locks and any signals used to marshal threads (eg. done signals)
    //
    // allocate and initialize Dispatcher and Worker threads
    //
    // start your Workers
    //

    startTimer(&timer);

    //
    // start your Dispatcher
    //

    usleep(nmillisec);

    //
    // assert signals to stop Dispatcher
    //
    // call join on Dispatcher
    //
    // assert signals to stop Workers - they are responsible for leaving
    // the queues empty
    //
    // call join for each Worker
    //
    stopTimer(&timer);

    // report the total number of packets processed and total time


}



void *no_load_loop(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;

    block_on_start(thread->pool);

    hasht_t *table = thread->pool->table;

    while (!thread->pool->stop){

        sleep(1);
        fprintf(stderr, "LOOOPING");

    }
    
    return NULL;
}




