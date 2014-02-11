import csv
import sys

import matplotlib
matplotlib.use('Agg')

from pylab import *
from numpy import *
from scipy import optimize

i_npack = 1
i_nsour = 2
i_mean  = 3
i_uni   = 4
i_time  = 5


xaxis = ""
yaxis = ""
title = ""
label = ""

output = ""

plt = matplotlib.pyplot.figure(num=None, figsize=(15, 10), dpi=80, facecolor='w')
ax = axes()

def load(path):
    serial = []
    parallel = []
    sq = []
    with open(path, 'rb') as csvfile:
        for row in csvfile:

            row_list = row.strip().split()

            if row_list[1] == 's':
                serial.append(row.strip().split()[1:])

            if row_list[1] == 'p':
                parallel.append(row.strip().split()[1:])

            if row_list[1] == 'sq':
                sq.append(row.strip().split()[1:])

    return (array(serial), array(parallel), array(sq))

def experiment1():

    xaxis  = "Mean work/packet"
    yaxis  = "Speedup"
    title  = "Serial Queue Overhead"
    output = "experiment1.png"

    path   = "experiment1.dat"

    serial, parallel, sq = load(path)
    serial = array(serial)
    nPackets = serial[:, 1]
    nSources = serial[:, 2]
    mean = [25, 50, 100, 200, 400, 800]

    for n in [1, 8, 16]:
        serialTime = []
        parallTime = []
        mean = []
        for i in range(len(nSources)):
            if int(nSources[i]) == int(n):
                mean.append(float(serial[:, i_mean][i]))
                serialTime.append(float(serial[:, i_time][i]))
                parallTime.append(float(sq[:, i_time][i]))

        print "mean", mean
        print "serial", serialTime
        print "queued", parallTime

        label = "Speedup [n = %d]" % n
        x = mean
        y = array(serialTime) / array(parallTime)

        plot(x, y, "o-", alpha=.7, linewidth=3, label=label)

        print "MAX SPEEDUP EXP 1: ", max(y)

    # xticks(mean)
    # ax.set_xscale('log')

        packets = serial[:, i_npack]
        rate = []
        for i in range(len(mean)):
            rate.append (  float(packets[i]) / serialTime[i]  ) 

        print zip(mean, map(int, rate))

    # print zip(mean, 

    ax.set_xlabel(xaxis) 
    ax.set_ylabel(yaxis)
    ax.set_title(title)


    return output

def fita(x):

    return 256432 * x ** -.44

def fit1(x):

    return 0.146238 *x**3 - 8.606* x**2 + 115.5* x + 1398

def experiment2():

    xaxis  = "Number of sources"
    yaxis  = "Runtime Ratio [Packets/second]"
    title  = "Packets per Second vs Number of Sources"

    path   = "experiment2.dat"
    output = "experiment2.png"

    label = "Runtime ratio"

    serial, parallel, sq = load(path)
    
    npackets = [float(a[i_npack]) for a in parallel]
    nsources = [float(a[i_nsour]) for a in parallel]
    time     = [float(p[i_time]) for p in parallel]

    X = linspace(min(nsources), max(nsources), 100)

    print time
    print npackets
    print nsources

    y = array(nsources)*array(npackets)/time
    x = nsources

    print zip(map(int, x),map(int, y))
    
    xticks(x)

    plot(x, y, "o-", alpha=.7, linewidth=3, label=label)
    plot(X, fit1(X), "-", alpha=.7, linewidth=3, label="Dispatcher Rate Fit")

    ax.set_xlabel(xaxis) 
    ax.set_ylabel(yaxis)
    ax.set_title(title)

    return output



def experiment4():
    # ax.set_xscale('log')

    xaxis  = "Number of cores"
    yaxis  = "Speedup"
    title  = "Serial Queue Overhead"
    output = "experiment4.png"

    path   = "experiment4.dat"

    serial, parallel, sq = load(path)
    serial = array(serial)
    nPackets = serial[:, i_npack]
    nSources = serial[:, i_nsour]

    ns =  [1, 2, 4, 8, 15, 32, 64]
    means = [1000, 2000, 4000, 8000]

    # X = linspace(min(ns), 20, 100)

    for w in means:

        serialTime = []
        parallTime = []

        for i in range(len(nSources)):

            if int(serial[:, i_mean][i]) == int(w):
                
                serialTime.append(float(serial[:, i_time][i]))
                parallTime.append(float(parallel[:, i_time][i]))


        print "mean", means
        print "serial", serialTime
        print "parallel", parallTime

        label = "Speedup [W = %d]" % w
        x = ns
        y = array(serialTime) / array(parallTime)
        
        print x
        print y

        plot(x, y, "o-", alpha=.7, linewidth=3, label=label)
        print "MAX SPEEDUP EXP 4: ", max(y)


    print fita(8000)
    print fit1(8)

    W = 8000

    x = ns[0:-2]

    y = [ (x[i])*fita(W) / min((x[i])*fita(W), fit1(x[i])) for i in range(len(x))]

    plot(x, y, "k.-.", alpha=1, linewidth=2, label="Expected Speedup")


    xticks(ns)
    ax.set_xlabel(xaxis) 
    ax.set_ylabel(yaxis)
    ax.set_title(title)


    return output



def experiment5():
    # ax.set_xscale('log')

    xaxis  = "Mean work/packet"
    yaxis  = "Speedup"
    title  = "Serial Queue Overhead"
    output = "experiment5.png"

    path   = "experiment5.dat"

    serial, parallel, sq = load(path)
    serial = array(serial)
    nPackets = serial[:, i_npack]
    nSources = serial[:, i_nsour]


    ns =  [1, 2, 4, 8, 15, 32, 64]
    means = [1000, 2000, 4000, 8000]

    for w in means:

        serialTime = []
        parallTime = []

        for i in range(len(nSources)):

            if int(serial[:, i_mean][i]) == int(w):
                
                serialTime.append(float(serial[:, i_time][i]))
                parallTime.append(float(parallel[:, i_time][i]))


        print "mean", means
        print "serial", serialTime
        print "parallel", parallTime

        label = "Speedup [W = %d]" % w
        x = ns
        y = array(serialTime) / array(parallTime)
        
        print x
        print y

        print "MAX SPEEDUP EXP 5: ", max(y)
        plot(x, y, "o-", alpha=.7, linewidth=3, label=label)

    xticks(ns)
    ax.set_xlabel(xaxis) 
    ax.set_ylabel(yaxis)
    ax.set_title(title)

    return output


if sys.argv[1] == "1":
    output = experiment1()

if sys.argv[1] == "2":
    output = experiment2()

if sys.argv[1] == "4":
    output = experiment4()

if sys.argv[1] == "5":
    output = experiment5()


lgd = ax.legend( bbox_to_anchor=(1., 1.), loc="best",
                 ncol=1, fancybox=True, shadow=True)
                 # borderaxespad=1.).get_frame().set_alpha(0.1)

ax.yaxis.grid(color='gray', linestyle='dashed')
ax.xaxis.grid(color='gray', linestyle='dashed')

plt.tight_layout(pad=10)

handles, labels = ax.get_legend_handles_labels()
ax.grid('on')

print "saving to output: "+output

savefig(output, box_extra_artists=(lgd,), bbox_inches='tight')
