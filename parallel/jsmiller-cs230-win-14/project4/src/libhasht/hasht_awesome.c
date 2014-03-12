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

volatile int OPTIMISM = 32;

#define mask ((1<<table->logsize) - 1)

int hasht_awesome_add(hasht_t *table, void *item, int key)
{
    awesome_node_t *root = &table->awesome.root;
    int dimension = 0;
    int id;

    DEBUG("ADD [%d]", key);

    while (1){
        /* get a new hash position based in dimension */
        id = (key >> (dimension * table->logsize) ) & mask;

        DEBUG("ADD \t root->child [%p]", root->child);
        
        if (!root->child){
            DEBUG("ADD \t creating new child array, dimension %d", dimension);
            awesome_node_t *new = new_awesome_array(table->capacity);
            awesome_node_t *old = __sync_val_compare_and_swap(&root->child, NULL, new);
            /* somebody beat us to the punch */
            if (old) {
                /* maybe do something about this memory leak? for now,
                 * just drop the array */
            }
        }

        __sync_fetch_and_or(&root->child[id].residue, key);

        DEBUG("ADD \t [%d] inuse? [%d]", id, root->child[id].inuse);

        /* check if this slot is free, if so, claim it */
        if (__sync_val_compare_and_swap(&root->child[id].inuse, 0, 1)){

            DEBUG("ADD \t the desired location is taken [%d]", id);

            for (int offset = 1; offset < OPTIMISM && id + offset < table->capacity; offset++){

                DEBUG("ADD \t \t checking [%d + %d]", id, offset);
                /* check if this slot is free, if so, claim it */
                if (!__sync_val_compare_and_swap(&root->child[id+offset].inuse, 0, 1)){
                    id += offset;
                    goto COMPLETE_ADD;
                }
            }

            /* if we are here, then our optimistic linear probe failed and try the next dimension*/
            dimension ++;
            root = &root->child[id];
            DEBUG("ADD \t this dimension is bunk, trying the next [%d]", dimension);

        } else {
            root->residue = ~0;
            goto COMPLETE_ADD;

        }

    }

COMPLETE_ADD:

    /* our atomic claim succeeded, proceed to insert the item */
    DEBUG("ADD \t found a free spot at [%d] in dimension [%d]", id, dimension);
    root->child[id].key   = key;
    root->child[id].data  = item;
    root->child[id].valid = 1;
    
    return 0;
}

void *hasht_awesome_remove(hasht_t *table, int key)
{
    awesome_node_t *root = table->awesome.root.child;
    void *data = NULL;
    int dimension = 0;
    int id;

    DEBUG("REMOVE [%d]", key);

    while (root){

        /* get a new hash position based in dimension */
        id = (key >> (dimension * table->logsize) ) & mask;

        if ( (root[id].residue & key) ^ key){
            return 0;
        }

        DEBUG("REMOVE \t [%d] in [%d] dimension [%d]?", key, id, dimension);

        for (int offset = 0; offset < OPTIMISM && id + offset < table->capacity; offset++){
            DEBUG("REMOVE \t \t checking [%d] at [%d]: %d", key, id, root[id+offset].key);
            if (root[id+offset].inuse && root[id+offset].key == key){
                DEBUG("REMOVE \t found [%d] at [%d]: %d", key, id, root[id+offset].key);
                if (!__sync_val_compare_and_swap(&root[id + offset].valid, 1, 0)){
                    data = root[id + offset].data;
                    root[id + offset].inuse = 0;
                }
                return data;
            }
        }
        
        dimension ++;
        root = root[id].child;

        DEBUG("REMOVE \t Failed. Trying [%d] on [%d], %p", id, dimension, root);

    }
    return data;

}


int hasht_awesome_contains(hasht_t *table, int key)
{
    awesome_node_t *root = table->awesome.root.child;
    int dimension = 0;
    int id;

    DEBUG("CONTAINS [%d]", key);

    while (root){

        /* get a new hash position based in dimension */
        id = (key >> (dimension * table->logsize) ) & mask;

        if ( (root[id].residue & key) ^ key){            
            return 0;
        }

        DEBUG("CONTAINS \t [%d] in [%d] dimension [%d]?", key, id, dimension);

        for (int offset = 0; offset < OPTIMISM && id + offset < table->capacity; offset++){
            DEBUG("CONTAINS \t \t checking [%d] at [%d]: %d", key, id, root[id+offset].key);
            if (root[id+offset].inuse && 
                root[id+offset].key == key && 
                root[id+offset].valid){
                DEBUG("CONTAINS \t found [%d] at [%d]: %d", key, id, root[id+offset].key);
                return 1;
            }
        }
        
        dimension ++;
        root = root[id].child;

        DEBUG("CONTAINS \t Failed. Trying [%d] on [%d], %p", id, dimension, root);

    }

    return 0;

}

void *hasht_awesome_resize(hasht_t *table)
{
    ERROR("THERE IS NO SUCH THING AS A RESIZE IN THE AWESOME LOCK!!!!");
    return NULL;
}

awesome_node_t *new_awesome_array(int size)
{
    awesome_node_t *new = MALLOC(awesome_node_t, size);
    memset(new, 0, size * sizeof(awesome_node_t));
    return new;
}

int hasht_awesome_init(hasht_t *table, int capacity, int expected_threads)
{
    DEBUG("initializing awesome table");

    table->capacity = capacity;
    table->logsize = (int) log2(table->capacity);

    fprintf(stderr, "Capacity: %d\tOptimism: %d\n", capacity, OPTIMISM);

    table->awesome.root.child   = new_awesome_array(table->capacity);
    table->awesome.root.residue = ~0;
    
    /* assign function pointers */
    table->add = &hasht_awesome_add;
    table->contains = &hasht_awesome_contains;
    table->remove = &hasht_awesome_remove;
    table->resize = &hasht_awesome_resize;

    return RET_SUCCESS;
}

void hasht_awesome_print(hasht_t *table, awesome_node_t *root, int dimension)
{
    if (!dimension) fprintf(stderr, "v--------- DUMPING AWESOME TABLE ---------v\n");
    
    if (root->child){
        char buf[1028] = {0};
        for(int j = 0; j < dimension; j++) strcat(buf, " | ");
        fprintf(stderr, "%s(%d):\t", buf, root->key);
        for(int i = 0; i < table->capacity; i++){
            if (root->child[i].inuse){
                fprintf(stderr, "[%d || %d] ", i, root->child[i].key);
            }
        }
        fprintf(stderr, "\n");
        for(int i = 0; i < table->capacity; i++)
            if (root->child[i].child)
                hasht_awesome_print(table, root->child + i, dimension+1);
    } 
    
    if (!dimension) fprintf(stderr, "^-----------------------------------------^\n");

    return;

}

