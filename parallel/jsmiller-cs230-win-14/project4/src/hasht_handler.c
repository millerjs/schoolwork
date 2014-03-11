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

double parallelDispatcher(hasht_type_t type, 
                          loop_t loop, 
                          int duration,
                          float fractionAdd,
                          float fractionRemove,
                          float hitRate,
                          int maxBucketSize,
                          long mean,
                          int initSize,
                          int capacity,
                          int nthreads,
                          int *nPackets,
                          double *elapsedTime)
{


    struct timeval tv;       
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

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

    thread_pool_start(pool);

    startTimer(&timer);

    long residue = 0;

    /* work until the day is done */
    unsigned long end = getms() + duration;
    while (getms() < end){
        for(int i = 0; i < nthreads; i++){
            
            if (ll_len(qs[i]) < QUEUE_DEPTH){
                HashPacket_t * pkt = getRandomPacket(source);
                ll_push(qs[i], pkt, mangleKey(pkt));
                residue += getFingerprint(pkt->body->iterations, pkt->body->seed);

            }

            *nPackets = *nPackets + 1;
        }

    }

    thread_pool_stop(pool);

    int ret = thread_pool_join(pool);

    stopTimer(&timer);

    *elapsedTime = *elapsedTime + getElapsedTime(&timer);

    return ret;

}

int get_next_queue(int nthreads)
{
    int id = (rand() * nthreads) / RAND_MAX;
    while (pthread_mutex_trylock(locks+id)){
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
        ll_Lamport_pop(qs[id]);
        pthread_mutex_unlock(locks+id);
    }
    
    return NULL;
}

void *worker_loop(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;
    
    block_on_start(thread->pool);

    hasht_t *table = thread->pool->table;
    
    while (!thread->pool->stop){
        int id = get_next_queue(thread->pool->size);
        HashPacket_t * pkt = (HashPacket_t*) ll_Lamport_pop(qs[id]);

        if (pkt){
            int key = mangleKey(pkt);
            switch(pkt->type) {
            case Add:
                table->add(table, (void*)pkt, key);
                break;
            case Remove:
                table->remove(table, key);
                break;
            case Contains:
                table->contains(table, key);
                break;
            }
        }

        pthread_mutex_unlock(locks+id);
    }
    
    return NULL;
}

