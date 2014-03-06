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


hasht_t *hasht_new(hasht_type_t type, int capacity, int expected_threads)
{
    const hasht_init_f inits[] = {
        &hasht_locking_init
    };
    
    ERROR_IF( type < 0 || type >= 1, "unknown lock type");
    
    hasht_t *table = MALLOC(hasht_t, 1);
    table->logsize = (int) log2(capacity);
    table->count   = 0;
    
    DEBUG("initialized table with logsize [%d]", table->logsize);

    inits[type](table, capacity, expected_threads);
    return table;
}



