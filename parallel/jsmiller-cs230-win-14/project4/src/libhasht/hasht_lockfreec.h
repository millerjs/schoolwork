/*****************************************************************************
 *                      
 *   FILE    : hasht_lockfree.h
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#ifndef _HASHT_LOCKFREEC_H
#define _HASHT_LOCKFREEC_H

#include "hasht.h"

int hasht_lockfreec_add(hasht_t *table, void *item, int key);
int hasht_lockfreec_contains(hasht_t *table, int key);
int hasht_lockfreec_init(hasht_t *table, int capacity, int expected_threads);
void hasht_lockfreec_print(hasht_t *table);
void *hasht_lockfreec_resize(hasht_t *table);
void *hasht_lockfreec_remove(hasht_t *table, int key);

#endif
