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

#include "cll.h"
#include "libhasht/hasht.h"
#include "hasht_handler.h"

#define QUEUE_DEPTH 16

#define MAX_THREADS 64

ll_t *qs[MAX_THREADS];
pthread_mutex_t locks[MAX_THREADS];


unsigned long getms()
{
    struct timeval tv;       
    if(gettimeofday(&tv, NULL) != 0) return 0;
    return (unsigned long)((tv.tv_sec * 1000ul) + (tv.tv_usec / 1000ul));
}


void parallelDispatcher(hasht_type_t type, 
                        loop_t loop, 
                        int nmillisec,
                        float fractionAdd,
                        float fractionRemove,
                        float hitRate,
                        int maxBucketSize,
                        long mean,
                        int initSize,
                        int capacity,
                        int nthreads)
{
    StopWatch_t timer;
	
    HashPacketGenerator_t * source = 
        createHashPacketGenerator(fractionAdd,fractionRemove,hitRate,mean);
    
    hasht_t *table = hasht_new(type, capacity, nthreads);
    for(int i = 0; i < initSize; i++){
        HashPacket_t * p = getAddPacket(source);
        table->add(table, (void*)p->body, mangleKey(p));
    }

    for (int i = 0; i < nthreads; i++){
        qs[i] = ll_new(QUEUE_DEPTH);
        pthread_mutex_init(locks+i, NULL);
    }

    thread_pool_t *pool = thread_pool_create(loop, nthreads);
    pool->table = table;

    // allocate and initialize locks and any signals used to marshal threads (eg. done signals)
    //
    // allocate and initialize Dispatcher and Worker threads

    thread_pool_start(pool);


    int nPackets = 0;
    /* HashPacket_t * pkt; */

    unsigned long end = getms() + nmillisec;

    startTimer(&timer);

    /* work until the day is done */
    while (getms() < end){
        usleep(10); 
        DEBUG("DISPATCH DISPATCH DISPATCH DISPATCH ");
        nPackets++;
    }
    
    usleep(nmillisec);

    thread_pool_stop(pool);

    thread_pool_join(pool);

    stopTimer(&timer);

    // report the total number of packets processed and total time

}

int get_next_queue(int nthreads)
{
    int id = (rand() * nthreads) / RAND_MAX;
    while (pthread_mutex_trylock(locks+id)){
        fprintf(stderr, "Trying to get queue [%d]\n", id);
        id = (rand() * nthreads) / RAND_MAX;
    }
    return id;
}

void *no_load_loop(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;

    block_on_start(thread->pool);

    while (!thread->pool->stop){
        int id = get_next_queue(thread->pool->size);
        HashPacket_t * packet = ll_Lamport_pop(qs[id]);
        fprintf(stderr, "Dropping packet [%p]\n", packet);
        pthread_mutex_unlock(locks+id);
    }
    
    return NULL;
}




