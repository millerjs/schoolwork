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

#include "thread_pool.h"
#include "util.h"
#include "cll.h"


ll_t * ll_new(int maxlen)
{
    ll_t * new = MALLOC(ll_t, 1);
    new->maxlen = maxlen;
    new->head = NULL;
    new->tail = NULL;
    ERROR_IF(!new, ERR_NOMEM);
    return new;
}

ll_node_t *ll_node_new(void *item)
{
   ll_node_t * new = MALLOC(ll_node_t, 1);
   new->data = item;
   new->next = NULL;
   return new;
}

int ll_push(ll_t *list, void *item)
{
    ERROR_IF(!list, ERR_NOMEM);
    if (list->maxlen > 0)
        if (list->len >= list->maxlen) 
            return ERR_LISTFULL;
    ll_node_t *new = ll_node_new(item);
    ll_node_t *tail = list->tail;
    list->tail = new;
    if (tail) 
        tail->next = new;    
    else
        list->head = new;
    list->len++;
    return RET_SUCCESS;
}

void *ll_pop(ll_t *list)
{
    ERROR_IF(!list, ERR_NOMEM);

    if (list->head == list->tail)
        return NULL;

    ll_node_t * head = list->head;
    list->head = list->head->next;
    void *data = head->data;
    free(head);

    return data;
}



