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

HashPacketGenerator_t *source = NULL;
int N_QS = 1;
long nPackets = 0;

void *dispatch_loop(void * __thread__);

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
                          int nthreads)
{

    N_QS = nthreads;
    StopWatch_t timer;
	
    source = createHashPacketGenerator(fractionAdd,fractionRemove,hitRate,mean);
    hasht_t *table = hasht_new(type, capacity, nthreads);
 
   for(int i = 0; i < initSize; i++){
        HashPacket_t * p = getAddPacket(source);
        table->add(table, (void*)p->body, mangleKey(p));
    }

    for (int i = 0; i < nthreads; i++){
        qs[i] = ll_new(QUEUE_DEPTH);
        qs[i]->len = 0;
        qs[i]->head = qs[i]->tail = NULL;
        pthread_mutex_init(locks+i, NULL);
    }

    /* setup threads */
    /* thread_pool_t *pool = thread_pool_create(loop, nthreads); */
    thread_pool_t *pool = thread_pool_create(no_load_loop, nthreads);
    thread_pool_t *dispatch = thread_pool_create(dispatch_loop, 1);
    pool->table = table;

    /* start experiment */
    thread_pool_start(dispatch);
    thread_pool_start(pool);
    startTimer(&timer);

    /* run for duration */
    usleep(duration*1000);

    /* stop threads */
    thread_pool_stop(pool);
    thread_pool_stop(dispatch);

    /* join threads */
    thread_pool_join(pool);
    thread_pool_join(dispatch);

    /* get time */
    stopTimer(&timer);
    double elapsedTime = getElapsedTime(&timer);

    /* calculate rate */
    double rate = nPackets / elapsedTime ;

    return rate;

}

void *dispatch_loop(void * __thread__)
{
    nPackets = 0;

    thread_t *thread = (thread_t*)__thread__;
    block_on_start(thread->pool);
    
    while (!thread->pool->stop){
        for(int i = 0; i < N_QS; i++){
            if (ll_len(qs[i]) < 100000){
                HashPacket_t * pkt = getRandomPacket(source);
                ll_push(qs[i], pkt, mangleKey(pkt));
                nPackets ++;
            }
        }
    }
    return NULL;
}

int get_next_queue(int nthreads)
{
    int id = rand() % nthreads;
    while (pthread_mutex_trylock(locks+id)){
        id = rand() % nthreads;
    }
    return id;
}

void *no_load_loop(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;

    block_on_start(thread->pool);

    while (!thread->pool->stop){
        ll_Lamport_pop(qs[thread->id]);
    }
    
    return NULL;
}

void *worker_loop(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;
    
    block_on_start(thread->pool);

    hasht_t *table = thread->pool->table;
    
    while (!thread->pool->stop){
        /* int id = get_next_queue(thread->pool->size); */
        HashPacket_t * pkt = (HashPacket_t*) ll_Lamport_pop(qs[thread->id]);

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

        /* pthread_mutex_unlock(locks+id); */
    }
    
    return NULL;
}

