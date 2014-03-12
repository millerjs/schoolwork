/*****************************************************************************
 *                      
 *   FILE    : main.c       
 *   DESCRIP : header file for threads
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "thread_pool.h"
#include "util.h"
#include "cll.h"
#include "../hash_utils/stopwatch.h"
#include "libhasht/hasht.h"

#include "experiments/experiments.h"

#define MAX_PATH 1028

extern volatile int START;
extern volatile int LOOP;

double stddev = 0;

int open(char*path, char mode);

FILE *open_output_file(int experiment)
{
    long sec = time(NULL);
    char output_path[MAX_PATH];
    sprintf(output_path, "/home/jsmiller/output/experiment_%d_%li.dat", experiment, sec);
    
    fprintf(stderr, "Writing output to [%s]\n", output_path);

    FILE *output_file = fopen(output_path, "w");
    if (!output_file){
        fprintf(stderr, "unable to open output file\n");
        exit(1);
    }
    return output_file;
}

void * thread_loop(void * __thread__)
{
    thread_t *thread = (thread_t*)__thread__;
    block_on_start(thread->pool);
    printf("thread: %li\n", thread->self);
    return NULL;
}

int main(int argc, char* argv[])
{
    int c;
    opterr = 0;
    int experiment = 0;
    FILE *output_file = NULL;

    float fracAdd = 0;
    float fracRem = 0;
    float hitRate = 0;
    int nthreads = 0;
    int type = 0;

    while ((c = getopt (argc, argv, "n:e:a:r:h:t:")) != -1){
        switch (c){
        case 'a':
            ERROR_IF(sscanf(optarg, "%f", &fracAdd) != 1, "-a expects an float option\n");
            break;
        case 'r':
            ERROR_IF(sscanf(optarg, "%f", &fracRem) != 1, "-r expects an float option\n");
            break;
        case 'h':
            ERROR_IF(sscanf(optarg, "%f", &hitRate) != 1, "-h expects an float option\n");
            break;
        case 't':
            ERROR_IF(sscanf(optarg, "%d", &type) != 1, "-t expects an int option\n");
            break;
        case 'n':
            ERROR_IF(sscanf(optarg, "%d", &nthreads) != 1, "-n expects an integer option\n");
            break;
        case 'e':
            ERROR_IF(sscanf(optarg, "%d", &experiment) != 1, "-e expects an integer option\n");
            break;
        default:
            fprintf(stderr, "Unknown argument -%c\n", c);
            break;
            
        }
    }

    ERROR_IF(!experiment, "Please specify experiment number [-e]\n");

    output_file = open_output_file(experiment);

    fprintf(stderr, "Running Experiment %d\n", experiment);
    
    if      (experiment == 1)     experiment1(output_file);
    else if (experiment == 2)     experiment2(output_file);
    else if (experiment == 3)     experiment3(output_file);
    else if (experiment == 4)     experiment4(output_file);
    else if (experiment == 7)     experiment7(output_file);
    else if (experiment == 5){
        ERROR_IF(!(fracAdd && fracRem && hitRate && nthreads), "exp5 needs options -artnh");
        experiment5(output_file, type, fracAdd, fracRem, hitRate, nthreads);
    } else if (experiment == 6){
        serialHashPacketTest(2000, .09, .01, .5, 32, 4000, 10);
        serialHashPacketTest(2000, .45, .05, .5, 32, 4000, 10);
        serialHashPacketTest(2000, .09, .01, .75, 32, 4000, 10);
        serialHashPacketTest(2000, .45, .05, .75, 32, 4000, 10);
        serialHashPacketTest(2000, .09, .01, .9, 32, 4000, 10);
        serialHashPacketTest(2000, .45, .05, .9, 32, 4000, 10);
        serialHashPacketTest(2000, .09, .01, .99, 32, 4000, 10);
        serialHashPacketTest(2000, .45, .05, .99, 32, 4000, 10);
    }
    else
        ERROR("unknown experiment number");

    fclose(output_file);

    return 0;
    
}


