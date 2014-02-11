/******************************************************************************/
/*                                                                            */
/* FILE    : locks.h                                                          */
/* DESCRIP : header file for threads                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 3                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#ifndef LOCKS_H_
#define LOCKS_H_

#include <pthread.h>

typedef unsigned char TAS_t;
typedef unsigned char TASB_t;

typedef struct ALOCK_t {
    int n;
    long tail;
    long curr;
    volatile int *flag;
} ALOCK_t;

/* typedef struct CLHnode_t CLHnode_t; */

/* 
 * struct CLHnode_t {
 *     long tail;
 *     long curr;
 *     volatile int *flag;
 * } CLHLock_t;
 */

typedef union Lock {
    TAS_t tas;
    TASB_t tasb;
    ALOCK_t alock;
    /* CLHLock_t clh; */
    pthread_mutex_t mutex;
} Lock;

/* Test and Set */
int TAS_init(Lock *tas);
int TAS_lock(Lock *tas);
int TAS_unlock(Lock *tas);

int mutex_lock(Lock *lock);
int mutex_unlock(Lock *lock);

int TASB_lock(Lock *lock);
int TASB_unlock(Lock *lock);
int TASB_init(Lock *lock);

int ALOCK_init(Lock *lock, int n);
int ALOCK_lock(Lock *lock);
int ALOCK_unlock(Lock *lock);

int CLHLock_init(Lock *lock);
int CLHLock_lock(Lock *lock);
int CLHLock_unlock(Lock *lock);




#endif

