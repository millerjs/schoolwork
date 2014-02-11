/******************************************************************************/
/*                                                                            */
/* FILE    : firewall.h                                                       */
/* DESCRIP : firewall header file                                             */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 2                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#ifndef FIREWALL_H_
#define FIREWALL_H_


#include "../../utils/generators.h"
#include "../../utils/stopwatch.h"
#include "../../utils/fingerprint.h"
#include "../../utils/packetsource.h"

/* User defined modules */
#include "q.h"
#include "util.h"
#include "threads.h"
#include "string.h"

extern long fingerprint;

double serialFirewall (int numPackets,
                       int numSources,
                       long mean,
                       int uniformFlag,
                       short experimentNumber);


double parallelFirewall (int numPackets, 
                         int numSources, 
                         long mean, 
                         int uniformFlag, 
                         short experimentNumber) ;

double serialQueueFirewall (int numPackets, 
                            int numSources, 
                            long mean, 
                            int uniformFlag, 
                            short experimentNumber) ;


#endif

