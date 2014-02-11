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

void error(char *fmt, ...);
void warnf(char *fmt, ...);


#endif

