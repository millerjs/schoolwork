/******************************************************************************/
/*                                                                            */
/* FILE    : tests.h                                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 1                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>

/* Class provided modules */
#include "../../utils/generators.h"
#include "../../utils/stopwatch.h"
#include "../../utils/fingerprint.h"
#include "../../utils/packetsource.h"

#include "tests.h"
#include "../src/firewall.h"
#include "../src/q.h"
#include "../src/threads.h"
#include "../src/util.h"


#define GREEN "\033[0;32m"
#define NORM "\033[0m"
#define RED "\033[0;31m"

int numSources = 8;
int numPackets = 300;
int mean       = 100;

int experimentNumber = 0;

#define FAILED 1
#define SUCCESS 0

int pthread_kill(pthread_t thread, int signal);

void result(int failed, char* test, char* subtest){

    if (failed){
        printf("TEST: [%20s] [%35s] [%s FAILED %s]\n", test, subtest, RED, NORM); 
        return;
    } 

    printf("TEST: [%20s] [%35s] [%s PASSED%s ]\n", test, subtest, GREEN, NORM); 


}

int testLQ1(){

    char *test = "testLQ";
    char *subtest = "basic enqueue and dequeue";

    int i, j; 
    volatile Packet_t packets[numPackets+1];
    
    LQ_t **qs = newLQ_array(numSources);

    for (i = 0; i < numSources; i ++){
        for (j = 0; j < numPackets+1; j ++){
            packets[j].iterations = numPackets - j;
            packets[j].seed = j;
            enqueue_LQ(qs[i], &packets[j]);
        }
    }

    volatile Packet_t temp;
    for (i = 0; i < numSources; i ++){
        for (j = 0; j < numPackets; j ++){

            if (dequeue_LQ(qs[i], &temp)){
                fprintf(stderr, "unable to dequeue packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }

            if (temp.iterations != packets[j].iterations){
                fprintf(stderr, "incorrect packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }

        }
    }
    result(SUCCESS, test, subtest);

    return SUCCESS;

}

int testLQ2(){

    char *test = "testLQ";
    char *subtest = "advanced enqueue and dequeue";

    volatile Packet_t temp;

    int i, j; 
    volatile Packet_t packets[numPackets];
    
    LQ_t **qs = newLQ_array(numSources);

    /* Part 1 */

    /* enqueue */
    for (i = 0; i < numSources; i ++){
        for (j = 0; j < numPackets/3+1; j ++){
            packets[j].iterations = numPackets - j;
            packets[j].seed = j;
            enqueue_LQ(qs[i], &packets[j]);
        }
    }

    /* dequeue */
    for (i = 0; i < numSources; i ++){
        for (j = 0; j < numPackets/3; j ++){
            if (dequeue_LQ(qs[i], &temp)){
                fprintf(stderr, "unable to dequeue packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }
            if (temp.iterations != packets[j].iterations){
                fprintf(stderr, "incorrect packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }

        }
    }

    /* Part 2 */

    /* enqueue */
    for (i = 0; i < numSources; i ++){
        for (j = numPackets/3+1; j < numPackets/3*2+1; j ++){
            packets[j].iterations = numPackets - j;
            packets[j].seed = j;
            enqueue_LQ(qs[i], &packets[j]);
        }
    }
    /* dequeue */
    for (i = 0; i < numSources; i ++){
        for (j = numPackets/3; j < numPackets/3*2; j ++){
            if (dequeue_LQ(qs[i], &temp)){
                fprintf(stderr, "unable to dequeue packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }
            if (temp.iterations != packets[j].iterations){
                fprintf(stderr, "incorrect packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }

        }
    }
    /* Part 3 */

    /* enqueue */
    for (i = 0; i < numSources; i ++){
        for (j = numPackets/3*2+1; j < numPackets+1; j ++){
            packets[j].iterations = numPackets - j;
            packets[j].seed = j;
            enqueue_LQ(qs[i], &packets[j]);
        }
    }
    /* dequeue */
    for (i = 0; i < numSources; i ++){
        for (j = numPackets/3*2; j < numPackets; j ++){
            if (dequeue_LQ(qs[i], &temp)){
                fprintf(stderr, "unable to dequeue packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }
            if (temp.iterations != packets[j].iterations){
                fprintf(stderr, "incorrect packet %d from queue %d\n", j, i);
                result(FAILED, test, subtest);
                return FAILED;
            }
        }
    }


    result(SUCCESS, test, subtest);

    return SUCCESS;

}

int testThreads1(){

    int i;
    char *test = "testThreads";
    char *subtest = "thread creation and joining";
    LQ_t **qs = newLQ_array(numSources);
    thread_arg_t **threads = create_thread_pool(numSources, qs, numPackets);    

    /* reset the threads upper limit on packets to tell them they're done */
    for (i = 0; i < numSources; i ++){
        threads[i]->npackets = -1;
    }

    /* sleep timeout to allow threads to realize they are done */
    sleep(1);
    /* All threads should have exited by now, if not, this test fails */

    for (i = 0; i < numSources; i ++){
        if (!pthread_kill(threads[i]->this, 0)){
            fprintf(stderr, "Thread %d is still alive but should have exited\n", i);
            result(FAILED, test, subtest);
            return FAILED;
        }
    }


    result(SUCCESS, test, subtest);

    return SUCCESS;


}

int compositeSmall(){

    char *test = "compositeTest";
    char *subtest = "serial/parallel/serialq (small)";

    int uniformFlag = 1;

    long fpar, fser, fsq;

    parallelFirewall (numPackets, 
                      numSources, 
                      mean, 
                      uniformFlag, 
                      experimentNumber) ;

    fpar = fingerprint;

    serialFirewall (numPackets, 
                      numSources, 
                      mean, 
                      uniformFlag, 
                      experimentNumber) ;

    fser = fingerprint;

    serialQueueFirewall (numPackets, 
                         numSources, 
                         mean, 
                         uniformFlag, 
                         experimentNumber) ;

    fsq = fingerprint;

    if (fpar != fser){
        fprintf(stderr, "parallel fingerprint does not match serial fingerprint\n");
        result(FAILED, test, subtest);
    }

    if (fsq != fser){
        fprintf(stderr, "serialq fingerprint does not match serial fingerprint\n");
        result(FAILED, test, subtest);
        return FAILED;
    }

    result(SUCCESS, test, subtest);

    return SUCCESS;

}

int compositeLarge(){

    char *test = "compositeTest";
    char *subtest = "serial/parallel/serialq (large)";

    int uniformFlag = 1;
    experimentNumber = 0;

    long fpar, fser, fsq;

    parallelFirewall (numPackets, 
                      numSources, 
                      mean, 
                      uniformFlag, 
                      experimentNumber) ;

    fpar = fingerprint;

    serialFirewall (numPackets, 
                      numSources, 
                      mean, 
                      uniformFlag, 
                      experimentNumber) ;

    fser = fingerprint;

    serialQueueFirewall (numPackets, 
                         numSources, 
                         mean, 
                         uniformFlag, 
                         experimentNumber) ;

    fsq = fingerprint;

    if (fpar != fser){
        fprintf(stderr, "parallel fingerprint does not match serial fingerprint\n");
        result(FAILED, test, subtest);
    }

    if (fsq != fser){
        fprintf(stderr, "serialq fingerprint does not match serial fingerprint\n");
        result(FAILED, test, subtest);
        return FAILED;
    }

    result(SUCCESS, test, subtest);

    return SUCCESS;

}



int main(int argc, char*argv[])
{

    int failed = 0;

    MAX_Q_LEN = 32;
    int i;
    fprintf(stdout, "Running all tests\n");

    for (i = 0; i < 5; i ++)
        failed += testLQ1();

    for (i = 0; i < 5; i ++)
        failed += testThreads1();
    
    for (i = 0; i < 5; i ++)
        failed += compositeSmall();

    numPackets = 10000;
    numSources = 100;
    
    for (i = 0; i < 5; i ++)
        failed += testLQ2();

    for (i = 0; i < 5; i ++)
        failed += compositeLarge();

    
    fprintf(stderr, "Finished all tests with [%d] errors\n", failed);


}

