/*****************************************************************************
 *                      
 *   FILE    : hasht..
 *   DESCRIP : definitions for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/


#include "hasht.h"
#include "util.h"
#include "math.h"
#include <pthread.h>

#include "hasht.h"
#include "hasht_lockfreec.h"
#include "hasht_locking.h"

int MAX_BUCKET_LEN = 1<<5;

hasht_t *hasht_new(hasht_type_t type, int capacity, int expected_threads)
{
    int ntypes = 2;
    const hasht_init_f inits[] = {
        &hasht_locking_init,
        &hasht_lockfreec_init,
    };
    
    ERROR_IF( type < 0 || type >= ntypes, "unknown lock type");
    
    hasht_t *table = MALLOC(hasht_t, 1);
    table->logsize = (int) log2(capacity);
    table->capacity = capacity;
    table->count   = 0;
    
    DEBUG("initialized table with logsize [%d]", table->logsize);

    inits[type](table, capacity, expected_threads);
    return table;
}



