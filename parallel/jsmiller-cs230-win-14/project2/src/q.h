/******************************************************************************/
/*                                                                            */
/* FILE    : util.h                                                           */
/* DESCRIP : header file for Lamport queue class                              */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 2                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#ifndef Q_H_
#define Q_H_

#include "../../utils/packetsource.h"
#include "../../utils/generators.h"
#include "../../utils/hashgenerator.h"
#include "../../utils/fingerprint.h"

typedef struct LamportQ_node LQ_node_t;

#define RET_SUCCESS 0
#define ERR_QFULL -1
#define ERR_NOSUCHQ -2
#define ERR_NOSUCHPACKET -2

extern int MAX_Q_LEN;

struct LamportQ_node{
    volatile Packet_t * packet;
    LQ_node_t * next;
};

typedef struct LamportQ{
    LQ_node_t *head;
    LQ_node_t *tail;
} LQ_t;


int getLQlen(LQ_t *q);
int enqueue_LQ(LQ_t *q, volatile Packet_t *packet);
int dequeue_LQ(LQ_t *q, volatile Packet_t *packet);

void fprintLQf(FILE *file, LQ_t *q);

LQ_t * newLQ();
LQ_t ** newLQ_array(int n);


#endif 



