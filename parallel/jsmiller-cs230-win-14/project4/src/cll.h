/******************************************************************************/
/*                                                                            */
/* FILE    : cll.c                                                            */
/* DESCRIP : header file for linked lists                                     */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 4                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/


#include <stdlib.h>
#include <unistd.h>

#include "thread_pool.h"
#include "util.h"

#define ERR_LISTFULL -2
#define ERR_LISTEMPTY -3

typedef struct ll_node_t ll_node_t;

struct ll_node_t {
    void *data;
    ll_node_t *next;
} ;

typedef struct ll_t {
    int len;
    int maxlen;
    ll_node_t *head;
    ll_node_t *tail;    
} ll_t; 


ll_t * ll_new(int maxlen);
int ll_push(ll_t *list, void *item);
void *ll_pop(ll_t *list);


