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
#include "../src/locks.h"

extern volatile int LOOP_THREADS;

typedef struct thread_arg_t {
    pthread_t this;
    int id;
    int loop;
    long test_counter;
    long *counter;
    Lock *lock;
    int (*acquire)(Lock*);
    int (*release)(Lock*);
} thread_arg_t;

/* The main worker loop for this project */
void * thread_loop(void* _args_);

/* Creates and spaws a pool of n threads */
thread_arg_t **create_thread_pool(int n);

/* Initializes n thread structures */
thread_arg_t **new_thread_arg_array(int n);

/* Joins all threads in array threads */
int join_thread_pool(thread_arg_t **threads, int n);

/* Set the functions to call when locking an unlocking */
int set_locking_functions(thread_arg_t ** threads, 
                          int (*acquire)(Lock*),
                          int (*release)(Lock*),
                          int n);

/* Insert the lock into the thread structures */
int set_thread_lock(thread_arg_t ** threads, 
                    Lock *lock,
                    int n);

void start_threads();
void stop_threads();

thread_arg_t **locking_thread_pool(Lock *lock, 
                                   int (*acquire)(Lock*),
                                   int (*release)(Lock*),
                                   long *counter,
                                   int n);



#endif

