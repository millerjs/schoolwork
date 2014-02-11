/******************************************************************************/
/*                                                                            */
/* FILE    : main.c                                                           */
/* DESCRIP : header file for threads                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 3                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include "threads.h"
#include "util.h"
#include "locks.h"

int main(int argc, char* argv[])
{

    if (argc != 3)
        error("./locks N_THREADS N_SECONDS");
    
    /* 
     * int n_threads = atoi(argv[1]);
     * int n_seconds = atoi(argv[2]);
     * 
     * int ret = 0;
     */

    Lock tas;
    TAS_init(&tas);

    TAS_lock(&tas);
    TAS_unlock(&tas);

    TAS_lock(&tas);

    fprintf(stderr, "\nExiting\n");

    return 0;
    
}


