/*****************************************************************************
 *
 *   FILE    : hasht_lockint.c
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#define _GNU_SOURCE
#include "hasht.h"
#include "hasht_linear.h"
#include "cll.h"

#define RESIZE_CONDITION (table->count > table->capacity / 2)

#define mask ((1<<table->logsize) - 1)

int hasht_linear_add(hasht_t *table, void *item, int key)
{

    int nlocks, table_size, bucket_id, lock_id, steps, ret;

ADD_RESTART:    

    nlocks     = table->linear.n_locks;
    table_size = table->capacity;
    bucket_id  = key & mask;
    lock_id    = bucket_id % nlocks;
    steps      = 1;
    ret        = 0;

    DEBUG("Add [%d] to [%d]", key, bucket_id);
    pthread_mutex_lock(table->linear.locks + lock_id);

    if (RESIZE_CONDITION){
        pthread_mutex_unlock(table->linear.locks + lock_id);
        table->resize(table);
        goto ADD_RESTART;
    }

    if (table->linear.resizing > 0){
        pthread_mutex_unlock(table->linear.locks + lock_id);
        usleep(1);
        goto ADD_RESTART;
    }

    if (table_size != table->capacity){
        pthread_mutex_unlock(table->linear.locks + lock_id);
        goto ADD_RESTART;
    }

    while ( table->linear.buckets[bucket_id].inuse ){
        int next_bucket = (bucket_id + 1) % table_size;
        int next_lock   = next_bucket % nlocks;

        ERROR_IF(steps > table->capacity, "table full! %d / %d", table->count, table->capacity);

        /* try and get next lock */
        while (pthread_mutex_trylock(table->linear.locks + next_lock)){
            if (table->linear.resizing > 0){
                /* if we failed, and someone else is resizing, rage quit */
                pthread_mutex_unlock(table->linear.locks + lock_id);
                usleep(1);
                goto ADD_RESTART;
            }
            /* hack to yeild thread */
            usleep(1);
        }

        /* We got the next lock, drop this one */
        pthread_mutex_unlock(table->linear.locks + lock_id);
        lock_id   = next_lock;
        bucket_id = next_bucket;
        steps++;

    }

    __sync_fetch_and_add(&table->count, 1);

    table->linear.buckets[bucket_id].data  = item;
    table->linear.buckets[bucket_id].key   = key;
    table->linear.buckets[bucket_id].inuse = 1;
    table->linear.buckets[key & mask].steps = MAX(table->linear.buckets[key & mask].steps, steps);

    DEBUG("Added [%d] to [%d] after [%d]", key, bucket_id, steps);

    pthread_mutex_unlock(table->linear.locks + lock_id);
    DEBUG("ADD released lock %d", lock_id);
    return ret;
}

void *hasht_linear_remove(hasht_t *table, int key)
{

    int nlocks, table_size, bucket_id, lock_id, max_steps, steps;
    void * data;

REMOVE_RESTART: 

    nlocks     = table->linear.n_locks;
    table_size = table->capacity;
    bucket_id  = key & mask;
    lock_id    = bucket_id % nlocks;
    max_steps  = table->linear.buckets[bucket_id].steps;
    steps      = 0;
    data     = NULL;

    /* DEBUG("Removes [%d] in [%d]?", key, bucket_id); */
    
    pthread_mutex_lock(table->linear.locks + lock_id);

    if (table->linear.resizing > 0){
        pthread_mutex_unlock(table->linear.locks + lock_id);
        usleep(1);
        goto REMOVE_RESTART;
    }

    if (table_size != table->capacity){
        pthread_mutex_unlock(table->linear.locks + lock_id);
        goto REMOVE_RESTART;
    }

    
    while ( table->linear.buckets[bucket_id].key != key ){
            /* && table->linear.buckets[bucket_id].inuse){ */

        /* DEBUG("   [?] Removes checking for [%d] in [%d]", key, bucket_id); */

        int next_bucket = (bucket_id + 1) % table_size;
        int next_lock   = next_bucket % nlocks;
        
        /* try and get next lock */
        while (pthread_mutex_trylock(table->linear.locks + next_lock)){

            if (table->linear.resizing > 0){
                DEBUG("   [?] removes on [%d] in a resize", key);
                /* if we failed, and someone else is resizing, rage quit */
                goto REMOVE_RESTART;
                usleep(1);
                return hasht_linear_remove(table, key);
            }
            usleep(1);
        }

        /* We got the lock, drop this one */
        pthread_mutex_unlock(table->linear.locks + lock_id);

        bucket_id = next_bucket;
        lock_id   = next_lock;
        
        if (steps++ > max_steps) 
            break;

    }
    
    /* if the element was found */
   if (table->linear.buckets[bucket_id].key == key){
        table->linear.buckets[bucket_id].inuse = 0;
        table->linear.buckets[bucket_id].data  = 0;
        table->linear.buckets[bucket_id].key   = 0;
        DEBUG("Removed [%d] from [%d]", 
              key, bucket_id);
    }
   
    __sync_fetch_and_sub(&table->count, 1);

    pthread_mutex_unlock(table->linear.locks + lock_id);
    
    return data;
}


int hasht_linear_contains(hasht_t *table, int key)
{
    int contains, nlocks, table_size, bucket_id, lock_id, max_steps, steps;

CONTAINS_RESTART:

    contains = 0;
    nlocks     = table->linear.n_locks;
    table_size = table->capacity;
    bucket_id  = key & mask;
    lock_id    = bucket_id % nlocks;
    max_steps  = table->linear.buckets[bucket_id].steps;
    steps      = 0;

    DEBUG("Contains [%d] in [%d] max [%d]?", key, bucket_id, max_steps);
    
    pthread_mutex_lock(table->linear.locks + lock_id);

    if (table->linear.resizing > 0){
        pthread_mutex_unlock(table->linear.locks + lock_id);
        usleep(1);
        goto CONTAINS_RESTART;
    }

    if (table_size != table->capacity){
        pthread_mutex_unlock(table->linear.locks + lock_id);
        goto CONTAINS_RESTART;
    }

    while ( table->linear.buckets[bucket_id].key != key ){
            /* && table->linear.buckets[bucket_id].inuse){ */

        /* DEBUG("   [?] Contains [%d], [%d] in [%d]: [%d]", steps, */
              /* key, bucket_id, table->linear.buckets[bucket_id].key); */

        int next_bucket = (bucket_id + 1) % table_size;
        int next_lock   = next_bucket % nlocks;

        /* DEBUG("CONTAINS grabbing lock %d", next_lock); */
        
        /* try and get next lock */
        while (pthread_mutex_trylock(table->linear.locks + next_lock)){

            if (table->linear.resizing > 0){
                DEBUG("   [?] contains on [%d] in a resize", key);
                /* if we failed, and someone else is resizing, rage quit */
                pthread_mutex_unlock(table->linear.locks + lock_id);
                usleep(1);
                goto CONTAINS_RESTART;
            }
            usleep(1);

        }

        /* We got the lock, drop this one */
        pthread_mutex_unlock(table->linear.locks + lock_id);
        /* DEBUG("CONTAINS released got lock %d", lock_id); */

        bucket_id = next_bucket;
        lock_id   = next_lock;

        if (steps++ > max_steps) break;
        
    }
    
    contains = ((table->linear.buckets[bucket_id].key == key) &&
                table->linear.buckets[bucket_id].inuse);

    /* 
     * if (!contains)
     * fprintf(stderr, "%d, %d : [%d] [%d]\n", bucket_id, key,
     *         table->linear.buckets[bucket_id].key, table->linear.buckets[bucket_id].inuse);
     */
    
    pthread_mutex_unlock(table->linear.locks + lock_id);
    
    if (table_size != table->capacity){
        return hasht_linear_contains(table, key);
    }

    DEBUG("   [?] Contains checking for [%d] in [%d]: [%d]", 
          key, bucket_id, table->linear.buckets[bucket_id].key);

    return contains;
}

void *hasht_linear_resize(hasht_t *table)
{
    __sync_fetch_and_add(&table->linear.resizing, 1);

    unsigned int incoming_capacity = table->capacity;
    unsigned int nlocks = table->linear.n_locks;

    for(int i = 0; i < nlocks; i++){
        pthread_mutex_lock(table->linear.locks + i);
    }

    DEBUG("Resize [%d / %d]", table->count, incoming_capacity);
    DEBUG(" ");
    DEBUG("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");

    
    if (incoming_capacity != table->capacity){
        /* somebody beat us to the resize */
        for(int i = 0; i < nlocks; i++){
            pthread_mutex_unlock(table->linear.locks + i);
        }
        __sync_fetch_and_sub(&table->linear.resizing, 1);
        return NULL;
    }

    table->capacity *= 2;
    table->logsize ++;
     
    linear_node_t *old_buckets = table->linear.buckets;
    table->linear.buckets = linear_buckets_new(table->capacity);

    for(unsigned int i_old = 0; i_old < incoming_capacity; i_old++){

        linear_node_t *old_bucket = old_buckets + i_old;
        
        if (old_bucket->inuse){

            int steps = 0;
            int key =  old_bucket->key;
            int i_new = key & mask;

            while ( table->linear.buckets[i_new].inuse ){
                i_new = (i_new + 1) % table->capacity;
                steps++;
            }

            DEBUG("   [*] moving [%d] from [%d] to [%d]", key, i_old, i_new);
            
            table->linear.buckets[key & mask].steps = 
                MAX(steps, table->linear.buckets[key & mask].steps);
            table->linear.buckets[i_new].data  = old_bucket->data;
            table->linear.buckets[i_new].key   = old_bucket->key;
            table->linear.buckets[i_new].inuse = 1;

        }        
        
        
    }


    DEBUG("resized table to [%d]", table->capacity);
    DEBUG("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    DEBUG(" ");

    for(int i = 0; i < nlocks; i++){
        pthread_mutex_unlock(table->linear.locks + i);
    }

    __sync_fetch_and_sub(&table->linear.resizing, 1);

    return old_buckets;
}


int hasht_linear_init(hasht_t *table, int capacity, int expected_threads)
{
    DEBUG("initializing linear table");

    int nlocks = expected_threads * 2;

    /* create locks */
    table->linear.locks   = MALLOC(pthread_mutex_t, nlocks);
    table->linear.n_locks = nlocks;
    table->linear.resizing = 0;

    for(int i = 0; i < nlocks; i++){
        pthread_mutex_init(&table->linear.locks[i], NULL);
    }

    /* create buckets */
    table->linear.buckets = linear_buckets_new(capacity);

    /* assign function pointers */
    table->add = &hasht_linear_add;
    table->contains = &hasht_linear_contains;
    table->remove = &hasht_linear_remove;
    table->resize = &hasht_linear_resize;

    return RET_SUCCESS;
}

void hasht_linear_print(hasht_t *table)
{
    fprintf(stderr, "table[%d/%d]: ", table->count, table->capacity);
    for(int i = 0; i < table->capacity; i++){
        if (table->linear.buckets[i].inuse)
            fprintf(stderr, "[%d(%d)|%d] ", i, table->linear.buckets[i].steps, 
                    table->linear.buckets[i].key);
    }
    fprintf(stderr, "\n");
}

linear_node_t *linear_buckets_new(int len)
{
    linear_node_t *new = MALLOC(linear_node_t, len);
    memset(new, 0, len*sizeof(linear_node_t));
    return new;
}
