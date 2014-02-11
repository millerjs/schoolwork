/******************************************************************************/
/*                                                                            */
/* FILE    : floyd.c                                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 1                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "stopwatch.h"
#include "util.h"
#include "floyd.h"


#undef DEBUG

int n = 0;

int n_threads = 1;
int checkout_size = 1;

volatile int next_row;
volatile int k_threaded;
volatile int go;

pthread_mutex_t lock_row;

int floyd_serial(int *m)
{

    /* fprint_matrix(stdout, m, n); */

    int i, j, k;

    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {

            if (i != k) {
                for (j = 0; j < n; j++) {

                    if (j != k) {

                        if (m[i _ j] > m[i _ k] + m[k _ j]) {
                            m[i _ j] = m[i _ k] + m[k _ j];

                        }
                    }
                }
            }
        }
    }


    /* fprint_matrix(stdout, m, n); */

    return 0;

}


int floyd_row(int *m, int k, int i)
{

    int j;
    for (j = 0; j < n; j++) {
        if (j != k) {

            if (m[i _ j] > m[i _ k] + m[k _ j]) {
                m[i _ j] = m[i _ k] + m[k _ j];

            }
        }
    }

    return 0;

}

void checkin(thread_t *t)
{
    pthread_mutex_unlock(&t->busy_m);

}

int checkout(thread_t *t)
{

    int row;

    if (!go) return WAIT_ON_GO;

    pthread_mutex_lock(&t->busy_m);

    pthread_mutex_lock(&lock_row);
    
    row = next_row;

    if (row >= n) {
        go = 0;
        row = ROWS_COMPLETE;
    } else {
        next_row += checkout_size;
    }
    
    pthread_mutex_unlock(&lock_row);

    return row;

}

void *thread_loop(void *_a_)
{
    thread_t *this = (thread_t *) _a_;
    int row, i, checkout_local;

    while (1) {

        if (k_threaded >= n) break;

        if (go) {

            row = checkout(this);

            if (row >= 0) {

                if (row + checkout_size >= n)
                    checkout_local = n - row;
                else 
                    checkout_local = checkout_size;

                /* This is the loop over i */
                for (i = 0; i < checkout_local; i++)
                    floyd_row(this->matrix, k_threaded, row + i);

            } 

            checkin(this);

        }

    }

    pthread_exit(NULL);

}

thread_t *new_thread(int *m, int id)
{

    thread_t *new_thread;

    if (!(new_thread = (thread_t *) malloc(sizeof(thread_t))))
        error("unable to allocate thread struct");
 
    new_thread->matrix = m;
    new_thread->id = id;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_mutex_init(&new_thread->busy_m, NULL))
        error("unable to initialize thread busy lock");

    int ret = pthread_create(&new_thread->p,
                             /* &attr, */
                             NULL,
                             &thread_loop,
                             new_thread);

    if (ret)
        error("unable to create thread: %s\n", strerror(ret));

    return new_thread;

}

void init_locks()
{
    if (!pthread_mutex_init(&lock_row, NULL))
        error("unable to initialize lock_row");
}


int floyd_parallel(int *m)
{

    next_row   = 0;
    k_threaded = 0;
    go         = 1;

    thread_t **threads = (thread_t**) malloc(n_threads*sizeof(thread_t*));

    int i;
    for (i = 0; i < n_threads; i++)
        threads[i] = new_thread(m, i);

    while (1) {

        while (next_row < n);

        /* Grab flow from threads */
        for (i = 0; i < n_threads; i++)
            pthread_mutex_lock(&threads[i]->busy_m);

        if (++k_threaded >= n)
            break;

        next_row = 0;

        /* Return flow to threads */
        for (i = 0; i < n_threads; i++)
            pthread_mutex_unlock(&threads[i]->busy_m);

        go = 1;

    }

    for (i = 0; i < n_threads; i++)
        pthread_mutex_unlock(&threads[i]->busy_m);

    void *st;
    for (i = 0; i < n_threads; i++)
        pthread_join(threads[i]->p, &st);

    free(threads);

    return 0;

}

