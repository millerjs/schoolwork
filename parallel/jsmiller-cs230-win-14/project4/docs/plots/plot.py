import csv
import sys

import matplotlib
matplotlib.use('Agg')

from pylab import *
from numpy import *
from scipy import optimize

xaxis  = "Mean work/packet"
yaxis  = "Speedup"
title  = "Lock Scaling"
output = "experiment1.png"

path   = "ex3loop.dat"

plt = matplotlib.pyplot.figure(num=None, figsize=(15, 10), dpi=80, facecolor='w')
ax = axes()


none = []
CLH = []
TAS = []
TASB = []
mutex = []
ALOCK = []
with open(path, 'rb') as csvfile:
    for row in csvfile:

        row_list = row.strip().split()
        print row_list

        if row_list[0] == 'none': none.append(row.strip().split()[1:])
        if row_list[0] == 'CLH': CLH.append(row.strip().split()[1:])
        if row_list[0] == 'TAS': TAS.append(row.strip().split()[1:])
        if row_list[0] == 'TASB': TASB.append(row.strip().split()[1:])
        if row_list[0] == 'mutex': mutex.append(row.strip().split()[1:])
        if row_list[0] == 'ALOCK': ALOCK.append(row.strip().split()[1:])
                                
none = array(none)
CLH = array(CLH)
TAS = array(TAS)
TASB = array(TASB)
mutex = array(mutex)
ALOCK = array(ALOCK)


# for n in [1, 8, 16]:

    # plot(x, y, "o-", alpha=.7, linewidth=3, label=label)


# print zip(mean, 

ax.set_xlabel(xaxis) 
ax.set_ylabel(yaxis)
ax.set_title(title)

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
