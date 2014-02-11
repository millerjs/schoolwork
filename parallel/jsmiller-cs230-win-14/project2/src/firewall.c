#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../utils/generators.h"
#include "../../utils/stopwatch.h"
#include "../../utils/fingerprint.h"
#include "../../utils/packetsource.h"

#include "firewall.h"

long fingerprint = 0;

double serialFirewall (int numPackets,
                     int numSources,
                     long mean,
                     int uniformFlag,
                     short experimentNumber)
{
    PacketSource_t * packetSource = createPacketSource(mean, numSources, experimentNumber);
    StopWatch_t watch;
    fingerprint = 0;

    if( uniformFlag) {
        startTimer(&watch);
        for( int i = 0; i < numSources; i++ ) {
            for( int j = 0; j < numPackets; j++ ) {
                volatile Packet_t * tmp = getUniformPacket(packetSource,i);
                fingerprint += getFingerprint(tmp->iterations, tmp->seed);
            }
        }
        stopTimer(&watch);

    } else {
        startTimer(&watch);
        for( int i = 0; i < numSources; i++ ) {
            for( int j = 0; j < numPackets; j++ ) {
                volatile Packet_t * tmp = getExponentialPacket(packetSource,i);
                fingerprint += getFingerprint(tmp->iterations, tmp->seed);

            }
        }
        stopTimer(&watch);
    }

    return getElapsedTime(&watch);

}


double parallelFirewall (int numPackets, 
                           int numSources, 
                           long mean, 
                           int uniformFlag, 
                           short experimentNumber) 
{
 
    StopWatch_t watch;

    int packets_left = 1;
    LQ_t **qs                     = newLQ_array(numSources);
    thread_arg_t **threads        = create_thread_pool(numSources, qs, numPackets);
    PacketSource_t * packetSource = createPacketSource(mean, numSources, experimentNumber);

    startTimer(&watch);

    /* work until the day is done */
    while (packets_left){

        /* assume that we are done */
        packets_left = 0;
        
        /* loop through all sources */
        for (int i = 0; i < numSources; i++){

            /* if we can, add a new node to the queue */
            while (threads[i]->ncomplete < numPackets && getLQlen(qs[i]) < MAX_Q_LEN){
             
                if (uniformFlag)
                    enqueue_LQ(qs[i], getUniformPacket(packetSource, i));
                else
                    enqueue_LQ(qs[i], getExponentialPacket(packetSource, i));

            }
            
            /* should we restart this loop? */
            if (threads[i]->ncomplete < numPackets)
                packets_left = 1;

        }

    }

    /* wait for all threads to throw in the towel */
    join_thread_pool(numSources, threads);
    stopTimer(&watch);

    /* accumulate the fingerprint */
    fingerprint = 0;
    for (int i = 0; i < numSources; i++)
        fingerprint += threads[i]->fingerprint;

    return getElapsedTime(&watch);

}



double serialQueueFirewall (int numPackets, 
                            int numSources, 
                            long mean, 
                            int uniformFlag, 
                            short experimentNumber) 
{
 
    StopWatch_t watch;
    volatile Packet_t packet;
    int packets_left = 1;
    LQ_t **qs                     = newLQ_array(numSources);
    PacketSource_t * packetSource = createPacketSource(mean, numSources, experimentNumber);

    thread_arg_t ** threads = new_thread_arg_array(numSources);

    for (int i = 0; i < numSources; i ++){
        threads[i]->q = qs[i];
        threads[i]->ncomplete = 0;
        threads[i]->npackets = numPackets;
        
    }

    startTimer(&watch);

    /* work until the day is done */
    while (packets_left){

        /* assume that we are done */
        packets_left = 0;
        
        /* loop through all sources */
        for (int i = 0; i < numSources; i++){

            /* if we can, add a new node to the queue */
            while (threads[i]->ncomplete < numPackets && 
                   getLQlen(threads[i]->q) < MAX_Q_LEN){

                if (uniformFlag)
                    enqueue_LQ(threads[i]->q, getUniformPacket(packetSource, i));
                else
                    enqueue_LQ(threads[i]->q, getExponentialPacket(packetSource, i));



            }

            /* should we restart this loop? */
            if (threads[i]->ncomplete < numPackets)
                packets_left = 1;

        }
        
        /* Work on all the packets that we just queued. */
        for (int i = 0; i < numSources; i++){

            while (threads[i]->ncomplete < threads[i]->npackets &&
                   getLQlen(threads[i]->q) > 1){

                if (!dequeue_LQ(threads[i]->q, &packet)){
                    threads[i]->fingerprint += getFingerprint(packet.iterations, packet.seed);
                    threads[i]->ncomplete ++;
                }

            }
            
        }


    }

    stopTimer(&watch);

    /* accumulate the fingerprint */
    fingerprint = 0;
    for (int i = 0; i < numSources; i++)
        fingerprint += threads[i]->fingerprint;

    return getElapsedTime(&watch);

}
