/******************************************************************************/
/*                                                                            */
/* FILE    : threads.c                                                        */
/* DESCRIP : thread definitions                                               */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 2                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../../utils/generators.h"
#include "../../utils/stopwatch.h"
#include "../../utils/fingerprint.h"
#include "../../utils/packetsource.h"

#include "q.h"
#include "threads.h"
#include "util.h"

volatile int LOOP_THREADS = 1;

thread_arg_t **new_thread_arg_array(int n)
{

    int i;
    thread_arg_t **new = (thread_arg_t**) malloc(n*sizeof(thread_arg_t*));

    for (i = 0; i < n; i++){
        new[i] = malloc(sizeof(thread_arg_t));
        new[i]->q           = NULL;
        new[i]->id          = -1;
        new[i]->fingerprint = 0;
        new[i]->npackets    = 0;
        new[i]->ncomplete   = 0;
    }

    return new;

}

int join_thread_pool(int n, thread_arg_t **threads)
{
    int i;
    void *ret;
    for (i = 0; i < n; i++)
        pthread_join(threads[i]->this, &ret);

    return RET_SUCCESS;
}

thread_arg_t **create_thread_pool(int n, LQ_t **qs, int numPackets)
{

    int i;
    thread_arg_t **thread_args = new_thread_arg_array(n);
    pthread_t *threads = (pthread_t*) malloc(n*sizeof(pthread_t));

    if (!threads) 
        error("unable to allocate memory for threads");
    
    for (i = 0; i < n; i++){
        thread_args[i]->q = qs[i];        
        thread_args[i]->id = i;
        thread_args[i]->npackets = numPackets;
        thread_args[i]->this = threads[i];
    }
    
    for (i = 0; i < n; i++)
        if (pthread_create(&thread_args[i]->this, NULL, thread_loop, thread_args[i]))
            error("unable to create thread\n");

    return thread_args;

}

void * thread_loop(void* _args_)
{

    volatile Packet_t packet;
    thread_arg_t * args = (thread_arg_t*) _args_;

    if (!args->q) error("thread was passed a null queue");

    while (args->ncomplete < args->npackets){

        while (dequeue_LQ(args->q, &packet))
            if (args->ncomplete >= args->npackets) /* This is only for testing */
                return NULL;                       /* This is only for testing */

        args->fingerprint += getFingerprint(packet.iterations, packet.seed);
        args->ncomplete ++;

    }

    return NULL;

}
