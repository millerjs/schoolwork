/*****************************************************************************
 *                      
 *   FILE    : hasht_lockint.c
 *   DESCRIP : header file for hash tables
 *   AUTHOR  : Joshua Miller
 *   PROJECT : Project 4 
 *   CLASS   : Parallel Computing - Winter 2014
 *                                             
 ******************************************************************************/

#include "hasht.h"
#include "hasht_awesome.h"
#include "cll.h"

int OPTIMISM = 4;

#define mask ((1<<table->logsize) - 1)

int hasht_awesome_add(hasht_t *table, void *item, int key)
{

    awesome_node_t *root = &table->awesome.root;
    int level = 0;
    int id;

    hasht_awesome_print(table, root, 0);

ADD_RECURSE:

    /* get a new hash position based on level */
    id = (key >> level) & mask;
        
    if (!root->child){
        DEBUG("creating new child array, level %d", level);
        awesome_node_t *new = new_awesome_array(table->capacity);
        awesome_node_t *old = __sync_val_compare_and_swap(&root->child, NULL, new);
        /* somebody beat us to the punch */
        if (old) {
            WARN("about to leak memory while growing branch");
        }
    }
    
    if (root->child[id].inuse){
        DEBUG("the desired location is taken [%d]", id);
        
        for (int offset = 1; offset < OPTIMISM && id + offset < table->capacity; offset++){
            DEBUG("checking [%d] + [%d]", id, offset);
            if (!root->child[id + offset].inuse){
                
                
            }

        }

    }
        
    /* leave a hint that the key might be here */
    __sync_fetch_and_or(&root->residue, key);

    level++;
        

    return 0;
}

void *hasht_awesome_remove(hasht_t *table, int key)
{
    void *ret = NULL;
    

    return ret;
}


int hasht_awesome_contains(hasht_t *table, int key)
{
    int contains = 0;

    return contains;
}

void *hasht_awesome_resize(hasht_t *table)
{

    DEBUG("Resize [%d / %d]", table->count, incoming_capacity);
    DEBUG(" ");
    DEBUG("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");


    DEBUG("resized table to [%d]", new_capacity);
    DEBUG("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    DEBUG(" ");

    
    return NULL;
}

awesome_node_t *new_awesome_array(int size)
{
    awesome_node_t *new = MALLOC(awesome_node_t, size);
    memset(new, 0, sizeof(awesome_node_t)*size);
    return new;
}

int hasht_awesome_init(hasht_t *table, int capacity, int expected_threads)
{
    DEBUG("initializing awesome table");

    table->awesome.root.child = new_awesome_array(table->capacity);
    
    /* assign function pointers */
    table->add = &hasht_awesome_add;
    table->contains = &hasht_awesome_contains;
    table->remove = &hasht_awesome_remove;
    table->resize = &hasht_awesome_resize;

    return RET_SUCCESS;
}

void hasht_awesome_print(hasht_t *table, awesome_node_t *root, int level)
{
    if (!level)
        fprintf(stderr, "vvvvvvvvvv DUMPING AWESOME TABLE vvvvvvvvvv\n");
    
    if (root->child){

        for(int i = 0; i < table->capacity; i++){

            for(int j = 0; j < level; j++) fprintf(stderr, " | ");
            fprintf(stderr, "(%d): ", level);
            if (root->child[i].inuse)
                fprintf(stderr, "[%d : %d] ", i, root->child[i].key);
            if (root->child[i].child)
                hasht_awesome_print(table, root->child, level+1);
        }
        fprintf(stderr, "\n");

    } 
    
    if (!level)
        fprintf(stderr, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

    return;

}
