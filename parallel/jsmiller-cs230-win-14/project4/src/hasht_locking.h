/*****************************************************************************
 *                      
 *   FILE    : hasht_lockint.h
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#ifndef _HASHT_LOCKING_H
#define _HASHT_LOCKING_H


#include "hasht.h"


int hasht_locking_add(hasht_t *table, void *item);
int hasht_locking_contains(hasht_t *table, void *item);
int  hasht_locking_remove(hasht_t *table, void *item);
int  hasht_locking_resize(hasht_t *table);
int hasht_locking_init(hasht_t *table, int capacity, int expected_threads);


#endif
