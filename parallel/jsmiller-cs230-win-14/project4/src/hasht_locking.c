/*****************************************************************************
 *                      
 *   FILE    : hasht_lockint.c
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#include "hasht.h"
#include "hasht_locking.h"
#include "cll.h"

int hasht_locking_add(hasht_t *table, void *item)
{
    DEBUG("adding [%p] to table", item);
    return RET_SUCCESS;
}

int hasht_locking_contains(hasht_t *table, void *item)
{
    DEBUG("checking contains [%p] in table", item);
    return RET_SUCCESS;
}

int  hasht_locking_remove(hasht_t *table, void *item)
{
    DEBUG("removing [%p] from table", item);
    return RET_SUCCESS;
}

int  hasht_locking_resize(hasht_t *table)
{
    DEBUG("resizing table");
    return RET_SUCCESS;
}


int hasht_locking_init(hasht_t *table, int capacity, int expected_threads)
{
    DEBUG("initializing locking table");

    table->locking.buckets   = ll_new(MAX_Q_DEPTH);
    table->locking.rwlocks   = MALLOC(pthread_rwlock_t, expected_threads);
    table->locking.n_rwlocks = expected_threads;

    table->add = &hasht_locking_add;
    table->contains = &hasht_locking_contains;
    table->remove = &hasht_locking_remove;
    table->resize = &hasht_locking_resize;

    for(int i = 0; i < expected_threads; i++){
        pthread_rwlock_init(&table->locking.rwlocks[i], NULL);
    }

    return RET_SUCCESS;
}


