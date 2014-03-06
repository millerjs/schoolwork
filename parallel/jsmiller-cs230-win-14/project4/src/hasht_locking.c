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

int hasht_locking_add(hasht_t *table, int key, void *item)
{

    int bucket_id = table->logsize & key;
    int lock_id = bucket_id % table->locking.n_rwlocks;

    DEBUG("adding [%d] to bucket [%d]", key, bucket_id);
    
    pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);

    ll_push(table->locking.buckets[bucket_id], item);
    table->count ++;

    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    return RET_SUCCESS;
}

int hasht_locking_contains(hasht_t *table, int key)
{
    DEBUG("checking contains [%d] in table", key);

    int lock_id = (table->logsize & key) % table->locking.n_rwlocks;
    
    DEBUG("acquiring lock [%d]", lock_id);
    pthread_rwlock_rdlock(&table->locking.rwlocks[lock_id]);
    DEBUG("acquired lock [%d]", lock_id);


    DEBUG("releasing lock [%d]", lock_id);
    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    return RET_SUCCESS;
}

int  hasht_locking_remove(hasht_t *table, int key)
{
    DEBUG("removing [%d] from table", key);

    int lock_id = (table->logsize & key) % table->locking.n_rwlocks;
    
    DEBUG("acquiring lock [%d]", lock_id);
    pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);
    DEBUG("acquired lock [%d]", lock_id);


    DEBUG("releasing lock [%d]", lock_id);
    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);


    return RET_SUCCESS;
}

int  hasht_locking_resize(hasht_t *table)
{
    DEBUG("resizing table");

    int nlocks = table->locking.n_rwlocks;

    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        DEBUG("acquiring lock [%d]", lock_id);
        pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);
    }

    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        DEBUG("releasing lock [%d]", lock_id);
        pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);
    }

    return RET_SUCCESS;
}


int hasht_locking_init(hasht_t *table, int capacity, int expected_threads)
{
    DEBUG("initializing locking table");

    /* create buckets */
    table->locking.buckets = MALLOC(ll_t*, capacity);
    for(int i = 0; i < capacity; i++){
        table->locking.buckets[i] = ll_new(MAX_Q_DEPTH);
    }
    
    /* create locks */
    table->locking.rwlocks   = MALLOC(pthread_rwlock_t, expected_threads);
    table->locking.n_rwlocks = expected_threads;
    for(int i = 0; i < expected_threads; i++){
        pthread_rwlock_init(&table->locking.rwlocks[i], NULL);
    }

    /* assign function pointers */
    table->add = &hasht_locking_add;
    table->contains = &hasht_locking_contains;
    table->remove = &hasht_locking_remove;
    table->resize = &hasht_locking_resize;


    return RET_SUCCESS;
}


