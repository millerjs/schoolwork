/******************************************************************************/
/*                                                                            */
/* FILE    : floyd.h                                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 1                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#ifndef FLOYD_H
#define FLOYD_H


typedef struct thread{
    pthread_t p;
    pthread_mutex_t busy_m;
    int *matrix;
    int n;
    int id;
} thread_t;

extern int n;

extern int n_threads;
extern int checkout_size;

extern volatile int next_row;
extern volatile int k_threaded;
extern volatile int go;

extern pthread_mutex_t lock_row;


int floyd_serial(int *m);

int floyd_row(int *m, int k, int i);

void checkin(thread_t *t);

int checkout(thread_t *t);

void *thread_loop(void *_a_);

thread_t *new_thread(int *m, int id);

void init_locks();

int floyd_parallel(int *m);

int main(int argc, char *argv[]);

#endif 
