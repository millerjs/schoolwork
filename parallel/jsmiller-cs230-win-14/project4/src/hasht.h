/*****************************************************************************
 *                      
 *   FILE    : hasht.h
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

/* Thread pool definitions */

typedef void *(*loop_t)(void*);

typedef struct thread_pool_t thread_pool_t;

typedef struct thread_t {
    pthread_t self;
    thread_pool_t *pool;
} thread_t;

typedef struct thread_pool_t{
    thread_t *threads;
    int size;
    int start;
    int stop;
} thread_pool_t;

/* The top level hash table abstraction */
typedef struct hasht_t hasht_t;   

/* Constructor for new hash table */
hash_t *hasht_new(hasht_type_t type, int capacity, int expected_threads);

/* Pointer types for each table interaction member */
typedef int (*hasht_free_f)    (hasht_t *);
typedef int (*hasht_get_type_f)(hasht_t *);
typedef int (*hasht_resize_f)  (hasht_t *);
typedef int (*hasht_add_f)     (hasht_t *, void *);
typedef int (*hasht_remove_f)  (hasht_t *, void *);
typedef int (*hasht_contains_f)(hasht_t *, void *);

/* Enumeration to be contained in hasht object */
typedef enum hasht_type_t 
{
  LOCKING,
  LOCK_FREE,
  LINEAR_PROBE,
  AWESOME
} hasht_type_t;

struct hasht_t 
{
  /* Type specifier */
  hasht_type_t type;

  /* Member Function Pointers */
  hasht_add_f        add;
  hasht_contains_f   contains;
  hasht_remove_f     remove;
  hasht_resize_f     resize;

  /* Member types */
  hasht_locking_t      locking;
  hasht_lockfreec_t    lock_free;
  hasht_linearprobe_t  probing;
  hasht_awesome_t      awesome;
}; 
