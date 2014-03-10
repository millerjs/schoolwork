/*****************************************************************************
 *                      
 *   FILE    : hasht_lockint.h
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#ifndef _HASHT_LINEAR_H
#define _HASHT_LINEAR_H

#include "hasht.h"

int hasht_linear_add(hasht_t *table, void *item, int key);
int hasht_linear_contains(hasht_t *table, int key);
void *hasht_linear_remove(hasht_t *table, int key);
int hasht_linear_init(hasht_t *table, int capacity, int expected_threads);
void hasht_linear_print(hasht_t *table);
void *hasht_linear_resize(hasht_t *table);
linear_node_t *linear_buckets_new(int len);

#endif
