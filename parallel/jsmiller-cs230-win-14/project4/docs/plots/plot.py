import csv
import sys

import matplotlib
matplotlib.use('Agg')

from pylab import *
from numpy import *


def main():

    plt, ax = new_plot(xaxis="Optimism", yaxis="Throughput")
    
    root   = "plot4"
    path   = "plot4_7.dat"
    output = root+".png"
    
    x = []; y = []

    data = parse(path)

    sizes = [4, 8, 16, 32, 64, 128]

    throughput = 0
    capacity = 1
    optimism = 2
    threads = 3

    opt = 14
    
    for size in sizes:
        
        xindex = optimism
        yindex = throughput
        tindex = capacity

        x = array([ float(row[xindex]) for row in data if 
                    float(row[tindex]) == size 
                    ])

        y = array([ abs(float(row[0])) for row in data if 
                    float(row[tindex]) == size 
                    ])

        plot(x, y, "o-", alpha=.7, linewidth=3, label="Size: %d" % size)

        xlim(0, 17)

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
