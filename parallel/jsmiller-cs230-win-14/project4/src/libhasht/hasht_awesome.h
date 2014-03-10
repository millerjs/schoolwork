/*****************************************************************************
 *                      
 *   FILE    : hasht_lockint.h
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#ifndef _HASHT_AWESOME_H
#define _HASHT_AWESOME_H

#include "hasht.h"

extern volatile int OPTIMISM;

int hasht_awesome_add(hasht_t *table, void *item, int key);
int hasht_awesome_contains(hasht_t *table, int key);
void *hasht_awesome_remove(hasht_t *table, int key);
int hasht_awesome_init(hasht_t *table, int capacity, int expected_threads);
void hasht_awesome_print(hasht_t *table, awesome_node_t *root, int level);
void *hasht_awesome_resize(hasht_t *table);
awesome_node_t *new_awesome_array(int size);

int awesome_add_recurse(awesome_node_t *root, void *item, int key);
int awesome_remove_recurse(awesome_node_t *root, void *item, int key);
int awesome_contains_recurse(awesome_node_t *root, void *item, int key);

#endif
