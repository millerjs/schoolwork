/******************************************************************************
 *
 * FILE    : hasht_handler.c                              
 * DESCRIP : header file for threads
 * AUTHOR  : Joshua Miller
 * PROJECT : Project 3
 * CLASS   : Parallel Computing - Winter 2014
 *
 ******************************************************************************/

#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "../thread_pool.h"
#include "../util.h"
#include "../cll.h"
#include "../hasht_handler.h"

#include "../libhasht/hasht.h"


int experiment3(FILE *output)
{

    hasht_type_t types[] = {LOCKING, LOCKFREEC, LINEAR, AWESOME};
    float rates[] = {.5, .75, 0.9, 0.99};
    
    fprintf(output, "experiment \t type \t add \t remove \t hit" 
            "\t bsize \t mean \t init \t cap \t thread \t time \t rate\n");

    int ntimes   = 5;
    int duration = 2000;

    loop_t loop       = worker_loop;
    int maxBucketSize = 32;
    long mean         = 4000;
    int initSize      = 10;
    int capacity      = 32;

    for(int n = 1; n < 5; n++){
    

        for(int r = 0; r < 4; r++){
            
            float hitRate = rates[r];
        
            int nthreads = (1 << n) - 1;
        
            /* MOSTLY READS */

            for(int t = 0; t < 4; t++){

                hasht_type_t type    = types[t];
                float fractionAdd    = .09;
                float fractionRemove = .01;
                double rate = 0;

                for(int i = 0; i < ntimes; i++){
                    rate += parallelDispatcher(type, loop, duration, 
                                               fractionAdd, fractionRemove, 
                                               hitRate, maxBucketSize, 
                                               mean, initSize, capacity, 
                                               nthreads);
                    garbageCollect();
                }
    
                fprintf(output, "%d \t %d \t %f \t %f \t %f \t %d \t"
                        "%li \t %d \t %d \t %d \t %d \t %lf\n", 
                        2, type, fractionAdd, fractionRemove, 
                        hitRate, maxBucketSize, mean, 
                        initSize, capacity, nthreads, 
                        duration, rate/ntimes);
                fflush(output);

            }

            /* HEAVY WRITES */

            for(int t = 0; t < 4; t++){

                hasht_type_t type    = types[t];
                float fractionAdd    = .45;
                float fractionRemove = .05;

                double rate = 0;
                for(int i = 0; i < ntimes; i++){
                    rate += parallelDispatcher(type, loop, duration, 
                                               fractionAdd, fractionRemove, 
                                               hitRate, maxBucketSize, mean, 
                                               initSize, capacity, 
                                               nthreads);
                    garbageCollect();
                }
    
                fprintf(output, "%d \t %d \t %f \t %f \t %f \t %d \t"
                        "%li \t %d \t %d \t %d \t %d \t %lf\n", 
                        2, type, fractionAdd, fractionRemove, 
                        hitRate, maxBucketSize, mean, 
                        initSize, capacity, nthreads, 
                        duration, rate/ntimes);
                fflush(output);

            }
        }
    }

    return 0;

}





