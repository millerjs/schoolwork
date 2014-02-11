from subprocess import call
import time
import os

path = "/home/jsmiller/school/parallel/jsmiller-cs230-win-14/project2/firewall"
# path = "echo"
outputdir = "data"
output = outputdir+"/performance_"+time.strftime("%H_%M_%S")+".dat"
outf = open(output, 'w')
print "Writing benchmark output to: [%s]" % output

serial         = 1
parallel       = 2
serialQueue    = 3
serialParallel = 4

def runFirewall(path, 
                numPackets, 
                numSources, 
                mean, 
                uniformFlag, 
                experimentNumber, 
                D, 
                switch):

    cmd = []
    cmd.append(path)
    cmd.append(str(numPackets))
    cmd.append(str(numSources))
    cmd.append(str(mean))
    cmd.append(str(uniformFlag))
    cmd.append(str(experimentNumber))
    cmd.append(str(D))
    cmd.append(str(switch))
    print "Benchmark: " + " ".join(cmd)
    call(cmd, stdout=outf)

    
def experiment1():
    D           = 32
    means       = [25, 50, 100, 200, 400, 800]
    nsources    = [1, 8, 16]
    uniformFlag = 1
    expNum      = 1

    for w in means:
        for ns in nsources:
            np = 2**24/(ns*w)
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, serial)
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, serialQueue)

def experiment2():
    D           = 32
    means       = [1]
    nsources    = [1, 2, 4, 8, 16, 32]
    uniformFlag = 1
    expNum      = 2
    
    for w in means:
        for ns in nsources:
            np = 2**20/ns
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, parallel)


def experiment3():
    D           = 32
    means       = [1000, 2000, 4000, 8000]
    nsources    = [1, 2, 4, 8, 16, 32, 64]
    uniformFlag = 1
    expNum      = 3

    for w in means:
        for ns in nsources:
            np = 2**24/(ns*w)
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, serial)
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, parallel)


def experiment4():
    D           = 32
    means       = [1000, 2000, 4000, 8000]
    nsources    = [1, 2, 4, 8, 16, 32, 64]
    uniformFlag = 1
    expNum      = 4

    for w in means:
        for ns in nsources:
            np = 2**17
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, serial)
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, parallel)


def experiment5():
    D           = 32
    means       = [1000, 2000, 4000, 8000]
    nsources    = [1, 2, 4, 8, 16, 32, 64]
    uniformFlag = 0
    expNum      = 5

    for w in means:
        for ns in nsources:
            np = 2**17
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, serial)
            runFirewall(path, np, ns, w, uniformFlag, expNum, D, parallel)


def experiment6():
    ds          = [4, 16, 64, 512, 2048]
    means       = [1000, 2000, 4000, 8000]
    nsources    = [1, 2, 4, 8, 16, 32, 64]
    uniformFlag = 0
    expNum      = 6

    for w in means:
        for ns in nsources:
            np = 2**17
            for D in ds:
                runFirewall(path, np, ns, w, uniformFlag, expNum, D, serial)
                runFirewall(path, np, ns, w, uniformFlag, expNum, D, parallel)
                runFirewall(path, np, ns, w, uniformFlag, expNum, D, serialQueue)


experiment1()
experiment2()
experiment4()
experiment5()

