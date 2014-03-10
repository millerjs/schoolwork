/*****************************************************************************
 *                      
 *   FILE    : hasht.h
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#ifndef _HASHT_H
#define _HASHT_H

#include <pthread.h>
#include "cll.h"

#define MAX_Q_DEPTH 16
#define MAX_TABLE_SIZE 1<<12

extern int MAX_BUCKET_LEN;

/* The top level hash table abstraction */
typedef struct hasht_t hasht_t;   

/* Pointer types for each table interaction member */
typedef int (*hasht_init_f)    (hasht_t *, int, int);
typedef int (*hasht_free_f)    (hasht_t *);
typedef int (*hasht_get_type_f)(hasht_t *);
typedef int (*hasht_add_f)     (hasht_t *, void*, int);
typedef int (*hasht_contains_f)(hasht_t *, int);
typedef void *(*hasht_remove_f)  (hasht_t *, int);
typedef void *(*hasht_resize_f)  (hasht_t *);

/* Enumeration to be contained in hasht object */
typedef enum hasht_type_t 
{
    LOCKING,
    LOCKFREEC,
    LINEAR,
    AWESOME
} hasht_type_t;

typedef struct hasht_locking_t
{
    ll_t **buckets;
    pthread_rwlock_t *rwlocks;
    int n_rwlocks;
} hasht_locking_t;

typedef struct hasht_lockfreec_t
{
    ll_t **buckets;
    pthread_rwlock_t *rwlocks;
    int n_rwlocks;
} hasht_lockfreec_t;

typedef struct linear_node_t{
    unsigned int key;
    unsigned char inuse;
    void *data;
    unsigned int steps;
} linear_node_t;

typedef struct hasht_linear_t
{
    int n_locks;
    int resizing;
    pthread_mutex_t *locks;
    linear_node_t *buckets;
} hasht_linear_t;

typedef struct awesome_node_t awesome_node_t;

struct awesome_node_t{
    unsigned int residue;
    unsigned int key;
    void *data;
    awesome_node_t *child;    
    unsigned char inuse;
};

typedef struct hasht_awesome_t
{
    awesome_node_t root;
 } hasht_awesome_t;

struct hasht_t 
{
    /* Type specifier */
    hasht_type_t type;

    /* Scalar properties */
    int logsize;
    int capacity;
    int count;

    /* Member Function Pointers */
    hasht_add_f      add;
    hasht_contains_f contains;
    hasht_remove_f   remove;
    hasht_resize_f   resize;

    /* Member types */
    hasht_locking_t      locking;
    hasht_lockfreec_t    lockfreec;
    hasht_linear_t       linear;
    hasht_awesome_t      awesome;
}; 


/* Constructor for new hash table */
hasht_t *hasht_new(hasht_type_t type, int capacity, int expected_threads);
int hasht_locking_init(hasht_t *table, int capacity, int expected_threads);


#endif
