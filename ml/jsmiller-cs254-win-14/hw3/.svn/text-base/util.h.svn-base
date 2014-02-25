 /******************************************************************************/
/*                                                                            */
/* FILE        : util.h                                                       */
/* AUTHOR      : Joshua Miller                                                */
/* DESCRIPTION : General utility functions and macros                         */
/*                                                                            */
/******************************************************************************/

#ifndef _UTIL_H
#define _UTIL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <setjmp.h>
#include <execinfo.h>

#define MAX_DESCRIPTOR 1028

#define __red__ "\033[1;31m"
#define __lgr__ "\033[1;32m"
#define __nrm__ "\033[0m"

#define STR(_x)   #_x
#define XSTR(_x)  STR(_x)

#ifdef DEBUG
#undef DEBUG
#endif

#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _b : _a; })

#define INFO_DUMP(msg)                                  \
    do {                                                \
        fprintf(stderr, "%s:%d: %s: %s: ",              \
                __FILE__,                               \
                __LINE__,                               \
                __func__,                               \
                msg);                                   \
    } while(0)                                          

#define ERROR(fmt, ...)                         \
    do {                                        \
        INFO_DUMP(__red__ "error" __nrm__);     \
        fprintf(stderr, fmt, ##__VA_ARGS__);    \
        fprintf(stderr, "\n");                  \
        exit(EXIT_FAILURE);                     \
    } while(0)

#define WARN(fmt, ...)                          \
    do {                                        \
        INFO_DUMP("warning");                   \
        fprintf(stderr, fmt, ##__VA_ARGS__);    \
        fprintf(stderr, "\n");                  \
    } while(0)

#define WARN_IF(f, fmt, ...){                   \
    do {                                        \
        if ((f) != 0){                          \
            WARN(fmt, ##__VA_ARGS__);           \
        }                                       \
    } while(0)

#define ERROR_IF(f, fmt, ...)                   \
    do {                                        \
        if ((f) != 0) {                         \
            ERROR(fmt, ##__VA_ARGS__);          \
        }                                       \
    } while(0)

    
#endif



