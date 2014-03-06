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


int hasht_locking_add(hasht_t *table, void *item, int key)
{

    unsigned int mask = (1 << table->logsize) - 1;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->locking.n_rwlocks;
    int ret = 0;

    if (table->locking.buckets[bucket_id]->len >= MAX_BUCKET_LEN -1){
        table->resize(table);
        mask = (1 << table->logsize) - 1;
        bucket_id = key & mask;
        lock_id = bucket_id % table->locking.n_rwlocks;
    }

    DEBUG("adding [%d] to bucket [%d]", key, bucket_id);
        
    pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);

    ret = ll_push(table->locking.buckets[bucket_id], item, key);
    
    __sync_fetch_and_add(&table->count, 1);

    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    return ret;
}

int hasht_locking_contains(hasht_t *table, int key)
{

    unsigned int mask = (1 << table->logsize) - 1;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->locking.n_rwlocks;
    int contains = 0;

    DEBUG("checking containment [%d] in bucket [%d]", key, bucket_id);
    
    pthread_rwlock_rdlock(&table->locking.rwlocks[lock_id]);

    contains = ll_contains(table->locking.buckets[bucket_id], key);

    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    return contains;
}

int hasht_locking_remove(hasht_t *table, int key)
{

    unsigned int mask = (1 << table->logsize) - 1;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->locking.n_rwlocks;
    void *ret = NULL;

    DEBUG("removing [%d] from bucket [%d]", key, bucket_id);
    
    pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);

    ret = ll_remove(table->locking.buckets[bucket_id], key);
    __sync_fetch_and_sub(&table->count, 1);

    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    return (!!ret);
}

void *hasht_locking_resize(hasht_t *table)
{
    int incoming_capacity = table->capacity;
    int nlocks = table->locking.n_rwlocks;

    DEBUG("resizing table from [%d]", incoming_capacity);

    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);
    }

    if (incoming_capacity != table->capacity)
        return NULL;
    
    int new_capacity = table->capacity * 2;
    int new_logsize  = table->logsize + 1;
    ll_node_t *temp  = NULL;
    int bucket_id    = 0;

    ll_t **buckets = MALLOC(ll_t*, new_capacity);
    ll_t **old_buckets = table->locking.buckets;

    for(int i = 0; i < new_capacity; i++) 
        buckets[i] = ll_new(MAX_BUCKET_LEN);

    for(int i = 0; i < table->capacity; i++){

        unsigned int mask = (1 << new_logsize) - 1;

        ll_push(table->locking.buckets[i], NULL, 0);
        while ((temp = ll_popnode(table->locking.buckets[i]))){
            temp->next = NULL;
            bucket_id = temp->key & mask;
            DEBUG("moving node [%d] from old[%d] to new[%d]", temp->key, i, bucket_id);
            ll_pushnode(buckets[bucket_id], temp);
        }
    }

    table->locking.buckets = buckets;
    table->capacity        = new_capacity;
    table->logsize         = new_logsize;

    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);
    }
    
    return old_buckets;
}


int hasht_locking_init(hasht_t *table, int capacity, int expected_threads)
{
    DEBUG("initializing locking table");

    /* create locks */
    table->locking.rwlocks   = MALLOC(pthread_rwlock_t, expected_threads);
    table->locking.n_rwlocks = expected_threads;
    for(int i = 0; i < expected_threads; i++){
        pthread_rwlock_init(&table->locking.rwlocks[i], NULL);
    }

    /* create buckets */
    table->locking.buckets = MALLOC(ll_t*, capacity);
    for(int i = 0; i < capacity; i++){
        DEBUG("adding list %d", i);
        table->locking.buckets[i] = ll_new(MAX_BUCKET_LEN);
    }

    /* assign function pointers */
    table->add = &hasht_locking_add;
    table->contains = &hasht_locking_contains;
    table->remove = &hasht_locking_remove;
    table->resize = &hasht_locking_resize;


    return RET_SUCCESS;
}

void hasht_locking_print(hasht_t *table)
{
    for(int i = 0; i < table->capacity; i++){
        fprintf(stderr, "bucket[%d] : ", i);
        ll_print(table->locking.buckets[i]);
        fprintf(stderr, "\n");
    }
}
