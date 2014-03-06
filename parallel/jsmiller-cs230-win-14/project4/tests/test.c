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

#include "../src/thread_pool.h"
#include "../src/hasht.h"
#include "../src/util.h"
#include "../src/cll.h"
#include "../../utils/hashgenerator.h"

#include "../src/hasht_locking.h"
#include "../src/hasht_locking.h"

#define GREEN "\033[0;32m"
#define NORM "\033[0m"
#define RED "\033[0;31m"

#define MAX_DESCRIP 1028
#define FAILED 1
#define PASSED 0

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
        if (!table->contains(table, keys[i]))
            return FAILED;
    for(int i = 0; i < n; i++)
        table->remove(table, keys[i]);
    for(int i = 0; i < n; i++)
        if (table->contains(table, keys[i]))
            return FAILED;
    return PASSED;
}

void *parallel_simple_loop(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;
    DEBUG("entered thread: %li", thread->self);
    thread->retval = PASSED;
    hasht_t *table = thread->pool->table;

    block_on_start(thread->pool);

    int iterations = 2;

    int n = 50;
    int keys[n];
    double data[n];

    for(int iter = 0; iter < iterations; iter++){

        srand(time(0));
        for(int i = 0; i < n; i++){
            keys[i] = rand();
            data[i] = (double) rand() / RAND_MAX;
            /* table->add(table, &data[i], keys[i]); */
        }

        for(int i = 0; i < n; i++){
            /* if (!table->contains(table, keys[i])) */
                thread->retval = FAILED;
        }


    }
    return thread->retval;
}

int parallel_simple_hash_test(hasht_type_t type)
{
    int nthreads = 4;
    int capacity = 8;
    thread_pool_t *pool = thread_pool_create(&parallel_simple_loop, nthreads);
    pool->table = hasht_new(type, capacity, nthreads);

    DEBUG("table %p", pool->table);
    exit(0);

    thread_pool_start(pool);
    thread_pool_join(pool);
    thread_pool_free(pool);

    return PASSED;
}


int serial_resize_hash_test(hasht_type_t type)
{

    int nthreads = 4;
    int capacity = 8;
    int iterations = 2;

    hasht_t *table = hasht_new(type, capacity, nthreads);

    int n = 500;

    int keys[n];
    double data[n];

    for(int iter = 0; iter < iterations; iter++){

        srand(time(0));
        for(int i = 0; i < n; i++){
            keys[i] = rand();
            data[i] = (double) rand() / RAND_MAX;
            table->add(table, &data[i], keys[i]);
        }

        for(int i = 0; i < n; i++){
            if (!table->contains(table, keys[i]))
                return FAILED;
        }

    }

    return PASSED;

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
    ll_push(list, NULL, 0);

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
    int test1 = 0;

    if (argc > 1){
        all = 0;
        if (!strcmp(argv[1], "all"))    all    = 1;
        if (!strcmp(argv[1], "test1"))  test1 = 1;
    }

    if (all){
        printf(" === Running all tests === \n");
    } else {
        printf(" === Running %s === \n", argv[1]);
    }

    if (all || test1){
        TEST(test_queue(), "serial enqueue and dequeue");
        TEST(serial_simple_hash_test(LOCKING), "test 1 on locking table");
        TEST(serial_resize_hash_test(LOCKING), "test 1 on locking table");
        TEST(parallel_simple_hash_test(LOCKING), "test 1 on locking table");

        /* 
         * TEST(simple_hash_test(LOCKFREEC), "test 1 on locking table");
         * TEST(resize_hash_test(LOCKFREEC), "test 1 on locking table");
         */
    }

    return 0;
}


