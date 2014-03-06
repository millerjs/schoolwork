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

#define GREEN "\033[0;32m"
#define NORM "\033[0m"
#define RED "\033[0;31m"

#define MAX_DESCRIP 1028
#define FAILED 1
#define PASSED 0

int serial_hash_test(hasht_type_t type)
{

    int nthreads = 4;
    int capacity = 32;

    hasht_t *table = hasht_new(type, capacity, nthreads);

    double fractionAdd    = .3;
    double fractionRemove = .3;
    double hitRate        = .4;
    long   mean           = 1000;

    HashPacketGenerator_t * source = createHashPacketGenerator(fractionAdd,
                                                               fractionRemove,
                                                               hitRate,
                                                               mean);
    int key = 5;
    double data = .0023;

    table->add(table, key, &data);
    table->contains(table, key);

    /* 
     * table->remove(table, key);
     * table->resize(table);
     */

    return PASSED;

}

int run_hash_test(hasht_type_t type)
{
    serial_hash_test(type);

    return uerr;
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
        ll_push(list, &values[i]);

    /* pop first third */
    for (int i = 0; i < n/3; i++){
        if (values[i] != *(int*)ll_pop(list))
            return FAILED;
    }

    /* push last half */
    for (int i = n/2; i < n; i++)
        ll_push(list, &values[i]);
    ll_push(list, NULL);

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
        TEST(run_hash_test(LOCKING), "test 1 on locking table");
    }

    return 0;
}


