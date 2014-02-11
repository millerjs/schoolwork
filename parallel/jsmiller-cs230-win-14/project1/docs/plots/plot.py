import csv
import sys

import matplotlib
matplotlib.use('Agg')

from pylab import *
from numpy import *
from scipy import optimize


# ------------------------------------------------------------------------
#                                 Setup
# ------------------------------------------------------------------------

plt = matplotlib.pyplot.figure(num=None, figsize=(15, 10), dpi=80, facecolor='w')
ax = axes()

path = "raw.dat"

xaxis = ""
yaxis = ""
title = ""

which = sys.argv[1:]

# ------------------------------------------------------------------------
#                               Load Data
# ------------------------------------------------------------------------

raw = []

with open(path, 'rb') as csvfile:
    for row in csvfile:
        raw.append([])
        for f in row.strip().split():
            raw[-1].append(float(f))
        

# ------------------------------------------------------------------------
#                                  Plot
# ------------------------------------------------------------------------

div = 7

serial = array(raw[0:div])
parallel = array(raw[div:])


sx = serial[:,0]
sy = serial[:,1]


print "using type %s" % which


if "overhead" in which:

    sizes =  [16, 32, 64, 128, 256, 512, 1024]
    ax.set_yscale('log')
    xticks(sizes)

    xaxis = "Problem Size"
    yaxis = "Execution Time (ms)"
    title = "Parallel Overhead Comparison"


    x = serial[:,0]
    y = serial[:,1]

    print "x: ", x
    print "y: ", y

    label = "Serial Data"
    plot(x, y, "o-", alpha=.7, linewidth=3, label=label)

    threads = 1

    x = []; y = []
    for i in range(len(parallel)):
        if parallel[i][1] == threads:
            if parallel[i][0] in x:
                
                ind = x.index(parallel[i][0])
                
                new = parallel[i][3]
                old = y[ind]
                
                print new, old
                y[ind] = min(new, old)
            else:
                x.append(parallel[i][0])
                y.append(parallel[i][3])
                
    print "x: ", x
    print "y: ", y


    label = "Parallel T=1"
    plot(x, y, "o-", alpha=.7, linewidth=3, label=label)


if "serial" in which:

    xaxis = "Problem Size"
    yaxis = "Execution Time (ms)"
    title = "Serial Execution Time vs Problem Size"
    label = "Serial Data"

    x = serial[:,0]
    y = serial[:,1]

    print "x: ", x
    print "y: ", y

    plot(x, y, "o-", alpha=.7, linewidth=3, label=label)

if "parallel" in which:
        


    if "thread" in which:

        index = which.index("thread")
        
        threads = int(which[index+1])

        xaxis = "Problem size"
        yaxis = "Execution Time"
        title = "Execution Time vs Problem Size [%d Threads]" % threads
        label = "Parallel Data"
        
        x = []; y = []
        for i in range(len(parallel)):
            if parallel[i][1] == threads:
                if parallel[i][0] in x:

                    ind = x.index(parallel[i][0])

                    new = parallel[i][3]
                    old = y[ind]

                    print new, old
                    y[ind] = min(new, old)
                else:
                    x.append(parallel[i][0])
                    y.append(parallel[i][3])
        
        print "x: ", x
        print "y: ", y
        
        plot(x, y, "o-", alpha=.7, linewidth=3, label=label)

    if "size" in which:
        
        # index = which.index("size")
        
        # size = int(which[index+1])
        
        sizes =  [16, 32, 64, 128, 256, 512, 1024]
        threads = [1, 3, 7, 15, 31, 63, 65]
        ax.set_yscale('log')
        xticks(threads)

        for ss in range(len(sizes)):
            
            size = sizes[ss]
        # for size in [1024]:

            xaxis = "Number of Work Threads" 
            yaxis = "Execution Time"
            title = "Execution Time vs Thread Count"
            label = "%d Nodes" % size
            
            x = []; y = []
            for i in range(len(parallel)):
                
                # if correct size
                if parallel[i][0] == size:
                    
                    if parallel[i][1] in x:
                        
                        ind = x.index(parallel[i][1])
                        
                        new = parallel[i][3]
                        old = y[ind]
                        
                        print new, old
                        y[ind] = min(new, old)

                        if min(new, old) == new:
                            print "new min chuck: ", parallel[i][2]
                        
                    else:
                        x.append(parallel[i][1])
                        y.append(parallel[i][3])
                        
            print "x: ", x
            print "y: ", y
                        
            plot(x, y, "o-", alpha=.5, linewidth=4, label=label)
        plot([x[0], x[-1]], [sy[-1],sy[-1]], "k-.", alpha=1, linewidth=4, label="Serial 1024 Nodes")
        

    if "performance" in which:
        
        # index = which.index("size")
        
        # size = int(which[index+1])
        
        sizes =  [16, 32, 64, 128, 256, 512, 1024]
        # sizes =  [16, 32]
        # ax.set_yscale('log')
        threads = [1, 3, 7, 15, 31, 63, 65]
        xticks(threads)
        # yticks(sizes)

        for ss in range(len(sizes)):
            
            size = sizes[ss]

            xaxis = "Thread Count"
            yaxis = "Speedup (Serial / Parallel)"
            title = "Execution Time vs Thread Count"
            label = "%d Nodes" % size
            
            x = []; y = []
            for i in range(len(parallel)):
                
                # if correct size
                if parallel[i][0] == size:
                    
                    if parallel[i][1] in x:
                        
                        ind = x.index(parallel[i][1])
                        
                        new = parallel[i][3]
                        old = y[ind]
                        
                        print new, old
                        y[ind] = min(new, old)

                        if min(new, old) == new:
                            print "new min chuck: ", parallel[i][2]
                        
                    else:
                        x.append(parallel[i][1])
                        y.append(parallel[i][3])
                        
            print "x: ", sx
            print "y: ", sy

            plot(x, sy[ss]/y, "o-", alpha=.5, linewidth=4, label=label)
            # plot(x, , "o-", alpha=.5, linewidth=4, label="Serial")


ax.set_xlabel(xaxis) 
ax.set_ylabel(yaxis)
ax.set_title(title)

lgd = ax.legend( bbox_to_anchor=(1., 1.), loc="best",
                 ncol=1, fancybox=True, shadow=True, 
                 borderaxespad=1.).get_frame().set_alpha(0.1)

ax.yaxis.grid(color='gray', linestyle='dashed')
ax.xaxis.grid(color='gray', linestyle='dashed')

plt.tight_layout(pad=10)

handles, labels = ax.get_legend_handles_labels()
ax.grid('on')

savefig("_".join(which)+".png", 
        box_extra_artists=(lgd,), bbox_inches='tight')
