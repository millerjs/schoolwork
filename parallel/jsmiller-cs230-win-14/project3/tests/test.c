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

#include "../src/threads.h"
#include "../src/util.h"
#include "../src/locks.h"

#define GREEN "\033[0;32m"
#define NORM "\033[0m"
#define RED "\033[0;31m"

#define MAX_DESCRIP 1028
#define FAILED 1
#define SUCCESS 0

int usleep(int msec);
int pthread_kill(pthread_t thread, int signal);

void result(int failed, char* test, char* subtest){

    if (failed){
        printf("TEST: [%20s] [%35s] [%s FAILED %s]\n", test, subtest, RED, NORM); 
        return;
    } 

    printf("TEST: [%20s] [%35s] [%s PASSED%s ]\n", test, subtest, GREEN, NORM); 

}


int test_TAS1(int n_tests){

    char *test = "TAS";
    char subtest[MAX_DESCRIP];
    sprintf(subtest, "consequctive %d trials", n_tests);
        
    Lock tas;
    TAS_init(&tas);

    for (int i = 0; i < n_tests; i++){
        TAS_lock(&tas);
        TAS_unlock(&tas);
    }

    result(SUCCESS, test, subtest);

    return SUCCESS;

}


int test_threaded(char* test, 
                  int n_tests, 
                  int n_threads,
                  Lock *lock,
                  int (*acquire)(Lock*),
                  int (*release)(Lock*))
{

    int k;

    char subtest[MAX_DESCRIP];
    sprintf(subtest, "%d threads %d trials", n_threads, n_tests);

    for (k = 0; k < n_tests; k ++){


        long counter = 0;

        thread_arg_t ** threads = locking_thread_pool(lock, 
                                                      acquire,
                                                      release,
                                                      &counter,
                                                      n_threads);

        start_threads();
        usleep(100000);
        stop_threads();

        join_thread_pool(threads, n_threads);

        int test_counter = 0;
        for (int i = 0; i < n_threads; i ++){
            test_counter += threads[i]->test_counter;
        }

        free(threads);

        if (test_counter - counter){
            result(FAILED, test, subtest);
            return 1;
        }

    }

    result(SUCCESS, test, subtest);

    return SUCCESS;

}

int test_backoff(){

    int reference[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int delay = 1;
    int delay_top = 1;
    for (int i = 0; i < 10; i ++){
        delay_top <<= 1;
        delay = rand()/(RAND_MAX/delay_top);
        if (delay_top != reference[i]){
            result(FAILED, "test_backoff", "verifying exponential backoff");
            return 1;
        }
    }

    result(SUCCESS, "test_backoff", "verifying exponential backoff");

    return 0;
    
}

int thread_battery(char *test,                   
                   Lock *lock,
                   int (*acquire)(Lock*),
                   int (*release)(Lock*))
{
    test_threaded(test, 1, 1, lock, acquire, release);
    test_threaded(test, 1, 2, lock, acquire, release);
    test_threaded(test, 1, 4, lock, acquire, release);
    test_threaded(test, 1, 8, lock, acquire, release);
    test_threaded(test, 1, 32, lock, acquire, release);
    test_threaded(test, 5, 8, lock, acquire, release);
    test_threaded(test, 10, 8, lock, acquire, release);
    test_threaded(test, 23, 8, lock, acquire, release);
    return 0;
}

int main(int argc, char* argv[])
{
    
    test_TAS1(100);
    test_backoff();

    Lock lock;

    /* 
     * TAS_init(&lock);
     * thread_battery("TAS", &lock, &TAS_lock, &TAS_unlock);
     * 
     * pthread_mutex_init(&lock.mutex, NULL);
     * thread_battery("pthread_mutex", &lock, &mutex_lock, &mutex_unlock);
     * 
     * TASB_init(&lock);
     * thread_battery("TASB", &lock, &TASB_lock, &TASB_unlock);
     */

    ALOCK_init(&lock, 32);
    thread_battery("ALOCK", &lock, &ALOCK_lock, &ALOCK_unlock);

    
}


