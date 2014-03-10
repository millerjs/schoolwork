/******************************************************************************
 *
 * FILE    : hasht_handler.h                           
 * DESCRIP : header file for threads
 * AUTHOR  : Joshua Miller
 * PROJECT : Project 3
 * CLASS   : Parallel Computing - Winter 2014
 *
 ******************************************************************************/

#ifndef _HASHT_HANDLER
#define _HASHT_HANDLER

#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "thread_pool.h"
#include "hasht_handler.h"
#include "util.h"
#include "cll.h"


#include "../hash_utils/crc32.h"
#include "../hash_utils/fingerprint.h"
#include "../hash_utils/generators.h"
#include "../hash_utils/hashgenerator.h"
#include "../hash_utils/hashpackettest.h"
#include "../hash_utils/hashpacketworker.h"
#include "../hash_utils/hashtable.h"
#include "../hash_utils/packetsource.h"
#include "../hash_utils/paddedprim.h"
#include "../hash_utils/seriallist.h"
#include "../hash_utils/statistics.h"
#include "../hash_utils/stopwatch.h"


typedef void *(*loop_t)(void*);

double parallelDispatcher(hasht_type_t type, 
                          loop_t loop, 
                          int duration,
                          float fractionAdd,
                          float fractionRemove,
                          float hitRate,
                          int maxBucketSize,
                          long mean,
                          int initSize,
                          int capacity,
                          int nthreads,
                          int *nPackets,
                          double *ellapsedTime);

int get_next_queue(int nthreads);

void *no_load_loop(void * __thread__);
void *worker_loop(void * __thread__);

#endif
