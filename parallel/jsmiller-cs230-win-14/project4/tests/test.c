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
#include "../src/util.h"
#include "../src/cll.h"

#define GREEN "\033[0;32m"
#define NORM "\033[0m"
#define RED "\033[0;31m"

#define MAX_DESCRIP 1028
#define FAILED 1
#define PASSED 0

int usleep(int msec);

int test_queue()
{
    ll_t *list = ll_new(-1);
    int values[] = {0, 3, 5, 13, -3};

    for (int i = 0; i < 5; i++)
        ll_push(list, &values[i]);
    ll_push(list, NULL);

    for (int i = 0; i < 5; i++){
        if (values[i] != *(int*)ll_pop(list))
            return FAILED;
    }

    return 0;

}

int run_test1(int delay)
{
    /* TIMEOUT     */

    TIMEOUT(1, usleep(RANDOM(0,delay)*1000));

    return uerr;

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

        /* TEST(run_test1(10000), "first test"); */
        TEST(test_queue(), "test on serial enqueue and dequeue");

    }

    return 0;
}


