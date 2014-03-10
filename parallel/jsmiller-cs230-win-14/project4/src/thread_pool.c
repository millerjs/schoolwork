/******************************************************************************/
/*                                                                            */
/* FILE    : threads.c                                                        */
/* DESCRIP : header file for threads                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 4                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/


#include <pthread.h>
#include <stdlib.h>

#include "cll.h"
#include "thread_pool.h"
#include "util.h"

int thread_pool_join(thread_pool_t *pool)
{
    void *ret;
    int retvalue = 0;
    for (int i = 0; i < pool->size; i++){
        pthread_join(pool->threads[i].self, &ret);
        if (ret) retvalue += *(int*)ret;
    }
    return retvalue;
}

void thread_pool_free(thread_pool_t *pool)
{
    free(pool->threads);
    free(pool);
    return;
}

thread_pool_t *thread_pool_create(loop_t loop, int n)
{
    thread_pool_t *pool = MALLOC(thread_pool_t, n);
    thread_t *threads = MALLOC(thread_t, n);

    pool->threads = threads;
    pool->size = n;

    thread_pool_stop(pool);
    
    for (int i = 0; i < n; i++){
        pool->threads[i].id = i;
        pool->threads[i].pool = pool;

        int rs = pthread_create(&pool->threads[i].self, NULL, loop, &pool->threads[i]);
        ERROR_IF(rs, "unable to create thread\n");
    }

    return pool;

}

void block_on_start(thread_pool_t *pool)
{
    while (!(pool->start))
        usleep(1);
}

void thread_pool_start(thread_pool_t *pool)
{
    pool->stop = 0;
    pool->start = 1;
}

void thread_pool_stop(thread_pool_t *pool)
{
    pool->stop = 1;
    pool->start = 0;
}
