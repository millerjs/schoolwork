import csv
import sys

import matplotlib
matplotlib.use('Agg')

from pylab import *
from numpy import *


def main():

    data = parse("data/eig2.dat")

    
    plt, ax = new_plot(title="Iterations for convergence", logx=True,
                       yaxis="Iterations", xaxis="$\epsilon$")
    
    root   = "plot1"
    path   = root+".dat"
    output = root+".png"

    sizes   = [10, 20, 40, 60, 80, 100, 1010, 5010, 9010, 15010]

    for size in sizes:
        epsilon = []; iterations = []
        for i in range(len(data)):
            if float(data[i][0]) == size:
                epsilon.append(float(data[i][1]))
                iterations.append(float(data[i][2]))

        epsilon = 1*10**(-array(epsilon))

        if len(iterations) > 0:
            print "(%.1f, %.1f)," %(size, iterations[-1] - 9)
        
        plot(epsilon, iterations, "o-", alpha=.7, 
             linewidth=3, label="$%d\\times %d$ matrix" % (size, size))

        if size == 10:
            fit = 0.780696*log(epsilon)-0.130952
            plot(epsilon, fit, "k-.", alpha=.7, 
             linewidth=3, label="Log fit: $%d\\times %d$ matrix" % (size, size))


    save_plot(plt, ax, output)

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
