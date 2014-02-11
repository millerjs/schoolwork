/******************************************************************************/
/*                                                                            */
/* FILE    : floyd.c                                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 1                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>

#include "stopwatch.h"
#include "util.h"
#include "floyd.h"

int main(int argc, char *argv[])
{

    char c;
    int use_random = 0;
    char *fin = NULL;
    char *fout = NULL;
    int use_parallel = 0;
    int use_serial = 0;

    while ((c = getopt(argc, argv, "pst:n:f:o:r:")) != -1)
        switch (c) {

        case 's':
            use_serial = 1;
            break;

        case 'p':
            use_parallel = 1;
            break;

        case 'r':
            use_random = 1;
            n = atoi(optarg);
            break;

        case 'n':
            checkout_size = atoi(optarg);
            break;

        case 'f':
            fin = strdup(optarg);
            break;

        case 't':
            n_threads = atoi(optarg);
            break;

        case 'o':
            fout = strdup(optarg);
            break;

        }

    if (!use_random && !use_serial)
        error("select either parallel or serial [-p/-s]");

    if (!use_random && !fin)
        error("please specify input file [-f ifile]");

    if (fout)
        fprintf(stderr, "Will be writing to file, eventually\n");

    int *original = NULL;

    int *m_p = NULL;
    int *m_s = NULL;

    if (use_random)
        original = random_matrix(n);
    else
        original = fread_adjacency(fin, &n);

    
    StopWatch_t serial, parallel;

    if (use_serial){

        m_s = copy_matrix(original, n);
        fprintf(stderr, "Using serial option\n");
        startTimer(&serial);
        floyd_serial(m_s);
        stopTimer(&serial);
        fprintf(stderr, "Serial time: %f\n", getElapsedTime(&serial));
        printf("serial\t%d\t%f\n", n, getElapsedTime(&serial));

    }


    if (use_parallel){

        /* checkout_size = n/8; */
        m_p = copy_matrix(original, n);
        fprintf(stderr, "Using parallel option\n");
        startTimer(&parallel);
        floyd_parallel(m_p);
        stopTimer(&parallel);
        fprintf(stderr, "Parallel time: %f\n", getElapsedTime(&parallel));
        printf("%d\t%d\t%d\t%f\n", n, n_threads, checkout_size, getElapsedTime(&parallel));

    }

    if (use_serial && use_parallel){

        compare_matrices(m_p, m_s, n);

    }

    return 0;

}

