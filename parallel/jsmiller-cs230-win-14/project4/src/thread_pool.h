/******************************************************************************/
/*                                                                            */
/* FILE    : threads.h                                                        */
/* DESCRIP : header file for threads                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 3                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/


#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>

typedef void *(*loop_t)(void*);

typedef struct thread_pool_t thread_pool_t;

typedef struct thread_t {
    pthread_t self;
    thread_pool_t *pool;
} thread_t;

typedef struct thread_pool_t{
    thread_t *threads;
    int size;
    int start;
    int stop;
} thread_pool_t;

/* Creates and spawns a pool of n threads */
thread_pool_t *thread_pool_create(loop_t loop, int n);

/* Joins all threads in array threads */
int thread_pool_join(thread_pool_t *threads);

/* Frees the thread pool and it's threads */
void thread_pool_free(thread_pool_t *pool);

/* Function to allow each thread to call it in order to wait for the
   thread's start flag */
void block_on_start();

/* Signal to all threads waiting on block_on_start() to continue */
void thread_pool_start(thread_pool_t *pool);

/* Signal to all threads looping on (!pool->stop) to stop */
void thread_pool_stop(thread_pool_t *pool);


#endif

