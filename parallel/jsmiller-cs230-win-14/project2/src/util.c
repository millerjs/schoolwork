/******************************************************************************/
/*                                                                            */
/* FILE    : util.c                                                           */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 2                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/


#include "util.h"

void error(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "\nERROR: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}

void warnf(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "\nWARNING: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
