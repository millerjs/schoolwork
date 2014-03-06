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
#include "hasht.h"
#include "cll.h"
#include "../../utils/stopwatch.h"

#define MAX_PATH 1028

extern volatile int START;
extern volatile int LOOP;

double stddev = 0;

int open(char*path, char mode);

FILE *open_output_file(int experiment)
{
    long sec = time(NULL);
    char output_path[MAX_PATH];
    sprintf(output_path, "output/experiment_%d_%li.dat", experiment, sec);
    
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

    while ((c = getopt (argc, argv, "n:e:")) != -1){
        switch (c){
        case 'n':
            break;
        case 'e':
            ERROR_IF(sscanf(optarg, "%d", &experiment) != 1,
                     "-e expects an integer option\n");
            break;
        default:
            fprintf(stderr, "Unknown argument -%c\n", c);
            break;
            
        }
    }

    ERROR_IF(!experiment, "Please specify experiment number [-e]\n");

    output_file = open_output_file(experiment);

    fprintf(stderr, "Running Experiment %d\n", experiment);
    
    if (experiment == 1)          ;
    else if (experiment == 2)     ;
    else if (experiment == 3)     ;
    else if (experiment == 4)     ;
    else if (experiment == 5)     ;
    else if (experiment == 6)     ;
    else if (experiment == 7)     ;
    else if (experiment == 8)     ;
    else
        ERROR("unknown experiment number");


    thread_pool_t *pool = thread_pool_create(&thread_loop, 10);

    thread_pool_start(pool);
    thread_pool_join(pool);
    thread_pool_free(pool);

    fclose(output_file);

    return 0;
    
}


