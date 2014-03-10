/******************************************************************************/
/*                                                                            */
/* FILE    : test.c                                                           */
/* DESCRIP : header file for threads                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 3                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "../hash_utils/hashgenerator.h"

#include "../src/thread_pool.h"
#include "../src/util.h"
#include "../src/cll.h"
#include "../src/libhasht/hasht.h"

#define GREEN "\033[0;32m"
#define NORM "\033[0m"
#define RED "\033[0;31m"

#define MAX_DESCRIP 1028
int FAILED = 1;
int PASSED = 0;

int serial_simple_hash_test(hasht_type_t type)
{
    int nthreads = 4;
    int capacity = 8;
    hasht_t *table = hasht_new(type, capacity, nthreads);
    int n = 5;
    int keys[] = {1, 16, 17, 31, 45};
    double data[] = {.234, 27.458, 2317.4, 458435.1346, 567123.8345};
    for(int i = 0; i < n; i++)
        table->add(table, &data[i], keys[i]);
    for(int i = 0; i < n; i++)
        if (!table->contains(table, keys[i])){
            WARN("should contain key[%d] = %d", i, keys[i]);
            return FAILED;
        }
    for(int i = 0; i < n; i++)
        table->remove(table, keys[i]);
    for(int i = 0; i < n; i++)
        if (table->contains(table, keys[i])){
            WARN("should not contain key[%d] = %d", i, keys[i]);
            return FAILED;
        }
    return PASSED;
}


int serial_resize_hash_test(hasht_type_t type)
{

    int nthreads = 4;
    int capacity = 4;

    hasht_t *table = hasht_new(type, capacity, nthreads);

    int n = 100;

    int keys[n];
    double data[n];


    srand(time(0));
    for(int i = 0; i < n; i++){
        keys[i] = rand();
        data[i] = (double) rand() / RAND_MAX;
        table->add(table, &data[i], keys[i]);
    }

    for(int i = 0; i < n; i++){
        if (!table->contains(table, keys[i])){
            WARN("should contain key[%d] = %d", i, keys[i]);
            return FAILED;
        }
    }

    return PASSED;

}


void *parallel_loop1(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;
    DEBUG("entered thread: %li", thread->self);
    void * ret = &PASSED;

    block_on_start(thread->pool);

    hasht_t *table = thread->pool->table;
    
    int n = 10;
    int keys[n];
    double data[n];
    
    for(int i = 0; i < n; i++){
        keys[i] = thread->self + i;
        data[i] = ((double) rand()) / RAND_MAX;
        table->add(table, &data[i], keys[i]);
    }
    
    for(int i = 0; i < n; i++){
        if (!table->contains(table, keys[i])){
            WARN("should contain key[%d] = %d", i, keys[i]);
            ret = &FAILED;
        }
    }

    return ret;
}

int parallel_hash_test1(hasht_type_t type)
{
    int nthreads = 4;
    int capacity = 8;
    int retvalue;

    MAX_BUCKET_LEN = 4;

    thread_pool_t *pool = thread_pool_create(&parallel_loop1, nthreads);
    pool->table = hasht_new(type, capacity, nthreads);

    thread_pool_start(pool);
    retvalue = thread_pool_join(pool);

    return retvalue;
}

void *parallel_loop2(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;
    DEBUG("entered thread: %li", thread->self);

    block_on_start(thread->pool);

    hasht_t *table = thread->pool->table;

    int n = 100;
    unsigned char contains[n];
    unsigned int key;
    double data[n];
    for(int i = 0; i < n; i++){
        contains[i] = 0;
        data[i] = ((double) rand()) / RAND_MAX;
    }

    srand(time(0));

    /* A pseudorandom full cycle over the test data */
    unsigned int index = rand() % n;
    unsigned int increment = 7;
    for(unsigned int i = 0; i < 4*n; ++i)
    {
        index = (index + increment) % n;

        key = 100*thread->id + index;

        if (contains[index]){

            /* make sure it's there when it's supposed to be */
            if  (!table->contains(table, key)){
                WARN("should contain key[%d]: %d", index, key);
                return &FAILED;
            }

            /* with some probability remove a random element from the table */
            if ( (double) rand() / RAND_MAX > .9){
                table->remove(table, key);
                contains[index] = 0;
            }

        } else  {
            /* Otherwise, make sure it's not there and add it */

            if (table->contains(table, key)){
                WARN("should not contain key[%d]: %d", index, key);
                return &FAILED;
            } 

            table->add(table, &data[index], key);
            contains[index] = 1;
        }



    }

    return &PASSED;
}

int parallel_hash_test2(hasht_type_t type)
{
    int ntests = 1000;
    int nthreads = 4;
    int capacity = 8;
    int retval = PASSED;
    
    MAX_BUCKET_LEN = 4;
    
    for(int i = 0; i < ntests; i++){
        thread_pool_t *pool = thread_pool_create(&parallel_loop2, nthreads);
        pool->table = hasht_new(type, capacity, nthreads);
        thread_pool_start(pool);
        if (thread_pool_join(pool)){
            WARN("test failed iteration %d", i);
            retval = FAILED;
        }
        garbageCollect();
    }

    return retval;
}



int test_queue()
{
    ll_t *list = ll_new(-1);
    const int n = 100;
    int values[n];

    srand(time(0));
    for (int i = 0; i < n; i++)
        values[i] = rand();

    /* push first half */
    for (int i = 0; i < n/2; i++)
        ll_push(list, &values[i], i);

    /* pop first third */
    for (int i = 0; i < n/3; i++){
        if (values[i] != *(int*)ll_pop(list))
            return FAILED;
    }

    /* push last half */
    for (int i = n/2; i < n; i++)
        ll_push(list, &values[i], 0);

    /* pop last two thirds */
    for (int i =n/3; i < n; i++){
        if (values[i] != *(int*)ll_pop(list))
            return FAILED;
    }
    
    return PASSED;
}


int main(int argc, char* argv[])
{

    int all = 1;
    int tests[10];
    memset(tests, 0, 10*sizeof(int));

    if (argc > 1){
        all = 0;
        if (!strcmp(argv[1], "all"))       all      = 1;
        if (!strcmp(argv[1], "QUEUE"))     tests[0] = 1;
        if (!strcmp(argv[1], "LOCKING"))   tests[1] = 1;
        if (!strcmp(argv[1], "LOCKFREEC")) tests[2] = 1;
        if (!strcmp(argv[1], "LINEAR"))    tests[3] = 1;
        if (!strcmp(argv[1], "AWESOME"))   tests[4] = 1;
    }

    if (all){
        printf(" === Running all tests === \n");
    } else {
        printf(" === Running %s === \n", argv[1]);
    }

    if (all || tests[0]){
        TEST(test_queue(), "serial enqueue and dequeue");
    }

    if (all || tests[1]){
        TEST(serial_simple_hash_test(LOCKING), "add/remove");
        TEST(serial_resize_hash_test(LOCKING), "add/remove/resize");
        TEST(parallel_hash_test1(LOCKING), "and/resize/contain");
        TEST(parallel_hash_test2(LOCKING), "random traversal of keyspace");
    }

    if (all || tests[2]){
        TEST(serial_simple_hash_test(LOCKFREEC), "add/remove");
        TEST(serial_resize_hash_test(LOCKFREEC), "add/remove/resize");
        TEST(parallel_hash_test1(LOCKFREEC), "and/resize/contain");
        TEST(parallel_hash_test2(LOCKFREEC), "random traversal of keyspace");
    }

    if (all || tests[3]){
        TEST(serial_simple_hash_test(LINEAR), "add/remove");
        TEST(serial_resize_hash_test(LINEAR), "add/remove/resize");
        TEST(parallel_hash_test1(LINEAR), "and/resize/contain");
        TEST(parallel_hash_test2(LINEAR), "random traversal of keyspace");
    }

    if (all || tests[4]){
        TEST(serial_simple_hash_test(AWESOME), "add/remove");
        TEST(serial_resize_hash_test(AWESOME), "add/remove/resize");
        TEST(parallel_hash_test1(AWESOME), "and/resize/contain");
        TEST(parallel_hash_test2(AWESOME), "random traversal of keyspace");
    }


    return 0;
}


