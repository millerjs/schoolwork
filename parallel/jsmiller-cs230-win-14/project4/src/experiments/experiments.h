/******************************************************************************
 *
 * FILE    : experiments.h 
 * DESCRIP : header file for threads
 * AUTHOR  : Joshua Miller
 * PROJECT : Project 3
 * CLASS   : Parallel Computing - Winter 2014
 *
 ******************************************************************************/

#ifndef _EXPERIMENTS
#define _EXPERIMENTS

#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "../thread_pool.h"
#include "../hasht_handler.h"
#include "../util.h"
#include "../cll.h"

#include "../libhasht/hasht.h"


int experiment1(FILE *output);
int experiment2(FILE *output);
int experiment3(FILE *output);
int experiment4(FILE *output);
int experiment5(FILE *output, hasht_type_t type, 
                float fractionAdd, float fractionRemove, 
                float hitRate, int nthreads);

#endif
