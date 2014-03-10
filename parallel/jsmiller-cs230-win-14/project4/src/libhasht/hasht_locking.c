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

#define RESIZE_CONDITION (table->locking.buckets[bucket_id]->len>=MAX_BUCKET_LEN)

int hasht_locking_add(hasht_t *table, void *item, int key)
{
    int table_size = table->capacity;
    unsigned int mask = (1 << table->logsize) - 1;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->locking.n_rwlocks;
    int ret = 0;


    pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);

    DEBUG("Add [%d] to [%d]", key, bucket_id);

    if (table->capacity != table_size){
        pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);
        return hasht_locking_add(table, item, key);
    }

    ret = ll_push(table->locking.buckets[bucket_id], item, key);
    __sync_fetch_and_add(&table->count, 1);
    DEBUG("   +++ added [%d] to [%d], len[%d]", 
          key, bucket_id, table->locking.buckets[bucket_id]->len);

    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    if (RESIZE_CONDITION){
        table->resize(table);
    }

    return ret;
}

void *hasht_locking_remove(hasht_t *table, int key)
{
    int table_size = table->capacity;
    unsigned int mask = (1 << table->logsize) - 1;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->locking.n_rwlocks;
    void *ret = NULL;

    pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);

    DEBUG("Remove [%d] from bucket [%d]", key, bucket_id);

    if (table_size != table->capacity){
        pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);
        return hasht_locking_remove(table, key);
    }

    ret = ll_remove(table->locking.buckets[bucket_id], key);
    __sync_fetch_and_sub(&table->count, 1);

    DEBUG("   --- removed [%d] from bucket [%d]", key, bucket_id);

    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    return ret;
}


int hasht_locking_contains(hasht_t *table, int key)
{

    unsigned int mask = (1 << table->logsize) - 1;
    int table_size = table->capacity;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->locking.n_rwlocks;
    int contains = 0;

    DEBUG("Contains [%d] in bucket [%d]", key, bucket_id);
    pthread_rwlock_rdlock(&table->locking.rwlocks[lock_id]);

    if (table_size != table->capacity){
        pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);
        return hasht_locking_contains(table, key);
    }

    contains = ll_contains(table->locking.buckets[bucket_id], key);
    DEBUG("   ??? contains result [%d] : %d", key, contains);

    pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);

    return contains;
}

void *hasht_locking_resize(hasht_t *table)
{
    int incoming_capacity = table->capacity;
    int nlocks = table->locking.n_rwlocks;
    DEBUG("Resize [%d / %d]", table->count, incoming_capacity);
    DEBUG(" ");
    DEBUG("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        pthread_rwlock_wrlock(&table->locking.rwlocks[lock_id]);
    }

    if (incoming_capacity != table->capacity){

        for (int lock_id = 0; lock_id < nlocks; lock_id ++){
            pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);
        }

        DEBUG("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        DEBUG(" ");
        return NULL;
    }

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

        while ((temp = ll_popnode(table->locking.buckets[i]))){
            temp->next = NULL;
            bucket_id = temp->key & mask;
            DEBUG("          moving node [%d] from old[%d] to new[%d]", temp->key, i, bucket_id);
            ll_pushnode(buckets[bucket_id], temp);
        }
    }

    table->locking.buckets = buckets;
    table->capacity        = new_capacity;
    table->logsize         = new_logsize;

    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        pthread_rwlock_unlock(&table->locking.rwlocks[lock_id]);
    }

    DEBUG("resized table to [%d]", new_capacity);
    DEBUG("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    DEBUG(" ");

    
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
    DEBUG("  | table[%p] : %d items in %d buckets", 
          table->locking.buckets, table->count, table->capacity);
    for(int i = 0; i < table->capacity; i++){
        if (table->locking.buckets[i]->len > 0){
            char buf[51];                                   
            int idx = ((unsigned int)pthread_self())%14;    
            snprintf(buf, 50, " ");
            fprintf(stderr, "%s%50s | %s",                  
                    __gry__, buf, colors[idx]);             
            fprintf(stderr, "  |   |  bucket[%d] : ", i);
            ll_print(table->locking.buckets[i]);
            fprintf(stderr, "\n");
        }
    }
}
