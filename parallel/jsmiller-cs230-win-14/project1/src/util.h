/******************************************************************************/
/*                                                                            */
/* FILE    : util.h                                                           */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 1                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

#define MAX_BUFF_LEN 2056
#define _ *n+

#define WAIT_ON_GO -2
#define ROWS_COMPLETE -1
#define INFINITY 1000000

void error(char *fmt, ...);

int fprint_matrix(FILE * f, int *m, int n);

int *copy_matrix(int *m, int n);

int *random_matrix(int n);

int compare_matrices(int *m1, int *m2, int n);

int *fread_adjacency(char *fin, int *n);


#endif

