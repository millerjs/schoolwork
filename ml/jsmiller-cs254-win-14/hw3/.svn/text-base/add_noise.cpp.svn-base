// ###########################################################################
// #  File: add_noise.cpp
// #  Josh Miller
// #  Homework 3 
// #  Machine Learning
// #  Winter 2014
// ###########################################################################
using namespace std;
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include <ctime>

#include "util.h"
#include "vector.h"
#include "matrix.h"

double theta = .0001;
double Z; 

#define N 100

matrix parseData()
{
    matrix ret(N, N, 0.0);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            ERROR_IF(!fscanf(stdin, "%lf", &ret[i][j]), "element read");
    ret.normalize();
    return ret;
}

int main (int argc, char *argv[])
{

    matrix y = parseData();

    
    
    return 0;
}

