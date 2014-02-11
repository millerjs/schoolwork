/******************************************************************************/
/*                                                                            */
/* FILE    : main.c                                                           */
/* DESCRIP : firewall main                                                    */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 2                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

/* Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Class provided modules */
#include "../../utils/generators.h"
#include "../../utils/stopwatch.h"
#include "../../utils/fingerprint.h"
#include "../../utils/packetsource.h"

/* User defined modules */
#include "q.h"
#include "util.h"
#include "threads.h"
#include "string.h"
#include "firewall.h"

/* Global definitions */
#define DEFAULT_NUMBER_OF_ARGS 8
#define N_TEST_RUNS 20

int main(int argc, char *argv[])
{
    int numPackets, numSources, uniformFlag, which;
    long mean;
    short experimentNumber;
    
    double time;

    int parallel = 0;
    int serial = 0;
    int serialqueue = 0;

    if(argc == DEFAULT_NUMBER_OF_ARGS) {
        numPackets       = atoi(argv[1]);
        numSources       = atoi(argv[2]);
        mean             = atol(argv[3]);
        uniformFlag      = atoi(argv[4]);
        experimentNumber = (short) atoi(argv[5]);
        MAX_Q_LEN        = atoi(argv[6]);
        which            = atoi(argv[7]);

        /* select which tests to run */
        if (which == 1)
            serial = 1;

        if (which == 2)
            parallel = 1;

        if (which == 3)
            serialqueue = 1;

    } 

    
    /* if the correct amount of arguments were not specified */
    else {
        fprintf(stderr, "%s %s",
                "./firewall numPackets numSources mean uniformFlag",
                "experimentNumber D [s=1/p=2/sq=3/s&p=4/sq&s=5]\n");
        exit(1);
    }


    if (serial){
        time = serialFirewall(numPackets, numSources, mean, uniformFlag, experimentNumber);
        printf("%d\t s\t%d\t%d\t%li\t%d\t%f\n", experimentNumber, numPackets, 
               numSources, mean, uniformFlag, time);

    }

    if (parallel){
        time = parallelFirewall(numPackets, numSources, mean, uniformFlag, experimentNumber);
        printf("%d\t p\t%d\t%d\t%li\t%d\t%f\n", experimentNumber, numPackets, 
               numSources, mean, uniformFlag, time);


    }

    if (serialqueue){
        time = serialQueueFirewall(numPackets, numSources, mean, uniformFlag, experimentNumber);
        printf("%d\t sq\t%d\t%d\t%li\t%d\t%f\n", experimentNumber, numPackets, 
               numSources, mean, uniformFlag, time);

    }


}


