/******************************************************************************/
/*                                                                            */
/* FILE    : locks.c                                                          */
/* DESCRIP : header file for threads                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 3                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "locks.h"

#define MAX_BACKOFF 1000

const int RAND_SCALE = RAND_MAX/10;

int TAS_lock(Lock *lock)
{
    while (__sync_lock_test_and_set(&lock->tas, 1));
    return 0;
}

int TAS_unlock(Lock *lock)
{
    return !__sync_lock_test_and_set(&lock->tas, 0);
}

int TAS_init(Lock *lock)
{
    return (lock->tas = 0);
}

int mutex_lock(Lock *lock)
{
    return pthread_mutex_lock(&lock->mutex);
}

int mutex_unlock(Lock *lock)
{
    return pthread_mutex_unlock(&lock->mutex);
}

int TASB_init(Lock *lock)
{
    srand(time(NULL));
    return (lock->tas = 0);
}

int TASB_lock(Lock *lock)
{
    int delay = 1;
    int delay_top = 1;
    while (__sync_lock_test_and_set(&lock->tas, 1)){
        delay_top <<= 1;
        delay = rand()/(RAND_SCALE/delay_top);
        usleep(delay);
    }
    return 0;
}

int TASB_unlock(Lock *lock)
{
    return !__sync_lock_test_and_set(&lock->tas, 0);
}

#define BLOCKED 1
#define FREE 0
#define PAD 8

int ALOCK_init(Lock *lock, int n)
{
    lock->alock.n = n*PAD;
    lock->alock.tail = 0;
    lock->alock.curr = 0;
    lock->alock.flag = (volatile int*) malloc(lock->alock.n*sizeof(int));
    for (int i = 1; i < lock->alock.n; i++)
        lock->alock.flag[i] = BLOCKED;
    lock->alock.flag[0] = FREE;
    return 0;
}

int ALOCK_free(Lock *lock)
{
    free(lock->alock.flag);
    return 0;
}


int ALOCK_lock(Lock *lock)
{

    int i = __sync_fetch_and_add(&lock->alock.tail, PAD) % lock->alock.n;
    while (lock->alock.flag[i] == BLOCKED);
    return 0;

}

int ALOCK_unlock(Lock *lock)
{

    int i = lock->alock.curr % lock->alock.n;
    lock->alock.flag[i] = BLOCKED;
    lock->alock.curr += PAD;
    i = lock->alock.curr % lock->alock.n;
    lock->alock.flag[i%lock->alock.n] = FREE;
    
    return 0;
}


