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

#include "libhasht/hasht.h"

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

void *no_load_loop(void * __thread__);


#endif
