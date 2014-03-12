import csv
import sys

import matplotlib
matplotlib.use('Agg')

from pylab import *
from numpy import *


def main():
    
    root   = "plot3"
    path   = "experiment_3_1394656398.dat"
    output = "plot3_%s.png" % sys.argv[1]
    
    data = parse(path)

    types = ["LOCKING", "LOCKFREEC", "LINEAR", "AWESOME"]

    loads = [ 
        [.09, .01, .5],
        [.09, .01, .75],
        [.09, .01, .9],
        [.09, .01, .99],
        [.45, .05, .5],
        [.45, .05, .75],
        [.45, .05, .9],
        [.45, .05, .99]
        ]

    load = loads[int(sys.argv[1])]

    plt, ax = new_plot(xaxis="Thread count", yaxis="Throughput", 
                       title="Experiment 3\nfracAdd: %.2f \nfracRemove: %.2f \nhitRate: %.2f" 
                       % (load[0], load[1], load[2]))

    for row in data:
        print row

    for t in [0, 1, 2, 3]:
        
        x = [ item[-3] for item in data if
              float(item[1]) == t       and
              float(item[2]) == load[0] and
              float(item[3]) == load[1] and 
              float(item[4]) == load[2]
              ]

        y = [ item[-1] for item in data if
              float(item[1]) == t       and
              float(item[2]) == load[0] and
              float(item[3]) == load[1] and 
              float(item[4]) == load[2]
              ]
        
        print types[t]
        print x

        x = array(x)
        y = array(y)

        plot(x/, y, "o-", alpha=.7, linewidth=3, label="Type: %s" % types[t])

    save_plot(plot, ax, output)
    

def save_plot(plt, ax, path):
    
    lgd = ax.legend( bbox_to_anchor=(1., 1.), loc="best",
                     ncol=1, fancybox=True, shadow=True)

    print "saving to output: "+path
    # savefig(output, box_extra_artists=(lgd,), bbox_inches='tight')
    savefig(path, bbox_inches='tight')


def new_plot(title = "", xaxis = "", yaxis = "", logx=False, logy=False):

    plt = matplotlib.pyplot.figure(num=None, figsize=(15, 10), dpi=80, facecolor='w')
    ax = axes()

    if logx: ax.set_xscale('log')
    if logy: ax.set_yscale('log')

    ax.set_xlabel(xaxis) 
    ax.set_ylabel(yaxis)
    ax.set_title(title)
    
    ax.yaxis.grid(color='gray', linestyle='dashed')
    ax.xaxis.grid(color='gray', linestyle='dashed')
    
    plt.tight_layout(pad=10)
    
    handles, labels = ax.get_legend_handles_labels()
    ax.grid('on')

    return plot, ax

# Parse data into list
def parse(path, sortCol = -1):
    data = []
    with open(path, 'rb') as csvfile:
        for row in csvfile:
            row_list = row.strip().split()
            data.append(row_list)

    if sortCol > 0:
        data = sorted(data, key=lambda a_entry: -a_entry[sortCol])

    return data

if __name__ == '__main__':
    main()
