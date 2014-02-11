/******************************************************************************/
/*                                                                            */
/* FILE    : threads.h                                                        */
/* DESCRIP : header file for threads                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 2                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/


#ifndef THREADS_H_
#define THREADS_H_

#include "pthread.h"

volatile extern int LOOP_THREADS;

typedef struct thread_arg_t {
    LQ_t *q;
    int id;
    pthread_t this;
    long int fingerprint;
    int ncomplete;
    int npackets;
} thread_arg_t;


void * thread_loop(void* _args_);
thread_arg_t **create_thread_pool(int n, LQ_t ** qs, int numPackets);
thread_arg_t **new_thread_arg_array(int n);
int join_thread_pool(int n, thread_arg_t **threads);

#endif

