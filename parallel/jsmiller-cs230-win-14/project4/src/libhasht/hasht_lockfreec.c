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
#include "hasht_lockfreec.h"

#define RESIZE_CONDITION (table->lockfreec.buckets[bucket_id]->len>=MAX_BUCKET_LEN)

int hasht_lockfreec_add(hasht_t *table, void *item, int key)
{
    int table_size = table->capacity;
    unsigned int mask = (1 << table->logsize) - 1;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->lockfreec.n_rwlocks;
    int ret = 0;


    pthread_rwlock_wrlock(&table->lockfreec.rwlocks[lock_id]);

    DEBUG("Add [%d] to [%d]", key, bucket_id);

    if (table->capacity != table_size){
        pthread_rwlock_unlock(&table->lockfreec.rwlocks[lock_id]);
        return hasht_lockfreec_add(table, item, key);
    }

    ret = ll_push(table->lockfreec.buckets[bucket_id], item, key);
    __sync_fetch_and_add(&table->count, 1);
    DEBUG("   +++ added [%d] to [%d], len[%d]", 
          key, bucket_id, table->lockfreec.buckets[bucket_id]->len);

    pthread_rwlock_unlock(&table->lockfreec.rwlocks[lock_id]);

    if (RESIZE_CONDITION){
        table->resize(table);
    }

    return ret;
}

void *hasht_lockfreec_remove(hasht_t *table, int key)
{
    int table_size = table->capacity;
    unsigned int mask = (1 << table->logsize) - 1;
    int bucket_id = key & mask;
    int lock_id = bucket_id % table->lockfreec.n_rwlocks;
    void *ret = NULL;

    pthread_rwlock_wrlock(&table->lockfreec.rwlocks[lock_id]);

    DEBUG("Remove [%d] from bucket [%d]", key, bucket_id);

    if (table_size != table->capacity){
        pthread_rwlock_unlock(&table->lockfreec.rwlocks[lock_id]);
        return hasht_lockfreec_remove(table, key);
    }

    ret = ll_remove(table->lockfreec.buckets[bucket_id], key);
    __sync_fetch_and_sub(&table->count, 1);

    DEBUG("   --- removed [%d] from bucket [%d]", key, bucket_id);

    pthread_rwlock_unlock(&table->lockfreec.rwlocks[lock_id]);

    return ret;
}


int hasht_lockfreec_contains(hasht_t *table, int key)
{

    unsigned int mask = (1 << table->logsize) - 1;
    int table_size = table->capacity;
    int bucket_id = key & mask;
    int contains = 0;

    DEBUG("Contains [%d] in bucket [%d]", key, bucket_id);

    contains = ll_contains(table->lockfreec.buckets[bucket_id], key);

    if (table_size != table->capacity){
        DEBUG("   ??? in resize [%d] in bucket [%d]", key, bucket_id);
        return hasht_lockfreec_contains(table, key);
    }

    DEBUG("   ??? contains result [%d] : %d", key, contains);

    return contains;
}

void *hasht_lockfreec_resize(hasht_t *table)
{
    int incoming_capacity = table->capacity;
    int nlocks = table->lockfreec.n_rwlocks;
    DEBUG("Resize [%d / %d]", table->count, incoming_capacity);
    DEBUG(" ");
    DEBUG("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        pthread_rwlock_wrlock(&table->lockfreec.rwlocks[lock_id]);
    }

    if (incoming_capacity != table->capacity){

        for (int lock_id = 0; lock_id < nlocks; lock_id ++){
            pthread_rwlock_unlock(&table->lockfreec.rwlocks[lock_id]);
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
    ll_t **old_buckets = table->lockfreec.buckets;

    for(int i = 0; i < new_capacity; i++) 
        buckets[i] = ll_new(MAX_BUCKET_LEN);

    for(int i = 0; i < table->capacity; i++){

        unsigned int mask = (1 << new_logsize) - 1;
        temp = table->lockfreec.buckets[i]->head;

        while (temp){
            ll_node_t *copy = ll_node_new(temp->data, temp->key);
            bucket_id = temp->key & mask;
            DEBUG("          moving node [%d] from old[%d] to new[%d]", 
                  temp->key, i, bucket_id);
            ll_pushnode(buckets[bucket_id], copy);
            temp = temp->next;
        }
    }

    table->lockfreec.buckets = buckets;
    table->capacity        = new_capacity;
    table->logsize         = new_logsize;

    for (int lock_id = 0; lock_id < nlocks; lock_id ++){
        pthread_rwlock_unlock(&table->lockfreec.rwlocks[lock_id]);
    }

    DEBUG("resized table to [%d]", new_capacity);
    DEBUG("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    DEBUG(" ");

    
    return old_buckets;
}


int hasht_lockfreec_init(hasht_t *table, int capacity, int expected_threads)
{
    DEBUG("initializing lockfreec table");

    /* create locks */
    table->lockfreec.rwlocks   = MALLOC(pthread_rwlock_t, expected_threads);
    table->lockfreec.n_rwlocks = expected_threads;
    for(int i = 0; i < expected_threads; i++){
        pthread_rwlock_init(&table->lockfreec.rwlocks[i], NULL);
    }

    /* create buckets */
    table->lockfreec.buckets = MALLOC(ll_t*, capacity);
    for(int i = 0; i < capacity; i++){
        DEBUG("adding list %d", i);
        table->lockfreec.buckets[i] = ll_new(MAX_BUCKET_LEN);
    }

    /* assign function pointers */
    table->add = &hasht_lockfreec_add;
    table->contains = &hasht_lockfreec_contains;
    table->remove = &hasht_lockfreec_remove;
    table->resize = &hasht_lockfreec_resize;

    return RET_SUCCESS;
}

void hasht_lockfreec_print(hasht_t *table)
{
    DEBUG("  | table[%p] : %d items in %d buckets", 
          table->lockfreec.buckets, table->count, table->capacity);
    for(int i = 0; i < table->capacity; i++){
        if (table->lockfreec.buckets[i]->len > 0){
            char buf[51];                                   
            int idx = ((unsigned int)pthread_self())%14;    
            snprintf(buf, 50, " ");
            fprintf(stderr, "%s%50s | %s",                  
                    __gry__, buf, colors[idx]);             
            fprintf(stderr, "  |   |  bucket[%d] : ", i);
            ll_print(table->lockfreec.buckets[i]);
            fprintf(stderr, "\n");
        }
    }
}
