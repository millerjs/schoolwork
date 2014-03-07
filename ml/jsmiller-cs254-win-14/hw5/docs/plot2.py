import csv
import sys

import matplotlib
matplotlib.use('Agg')

from scipy import optimize
from pylab import *
from numpy import *


def main():

    
    plt, ax = new_plot(title="Frobenius norm over $k$", 
                       yaxis="Frobenius norm", xaxis="$k$")
    
    root   = "plot2"
    path   = root+".dat"
    output = root+".png"

    data = array(parse("data/svd1.data"))
        
    plot(range(len(data)), data, "o-", alpha=.7, 
         linewidth=3, label="$\epsilon = 1\times10^{-10}$");

    dat = []
    for i in data:
        dat.append(float(data[0][0]))
    data = array(dat)
    # giving initial parameters
    C = Parameter(7)
    power = Parameter(3)
    B = Parameter(5)
    
    # define your function:
    def f(x): return  C() * x**power() + B()
    
    # fit! (given that data is an array with the data to fit)
    fit(f, [C, power, B], data)

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

class Parameter:
    def __init__(self, value):
            self.value = value

    def set(self, value):
            self.value = value

    def __call__(self):
            return self.value

def fit(function, parameters, y, x = None):
    def f(params):
        i = 0
        for p in parameters:
            p.set(params[i])
            i += 1
        return y - function(x)

    if x is None: x = arange(y.shape[0])
    p = [param() for param in parameters]
    optimize.leastsq(f, p)


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
