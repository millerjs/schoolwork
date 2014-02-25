// ###########################################################################
// #  File: vector.cpp
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

#include "vector.h"

vector::vector(int dim, double init) 
{
    if (dim <= 0){
        cerr << "vector cannot have non-positive dimensionality: " << dim << endl;
        exit(1);
    }
    d = dim;
    x = new double[d];
    for (int i = 0; i < d; i++)
        x[i] = init;
}

vector::vector(int dim) 
{
    if (dim <= 0){
        cerr << "vector cannot have non-positive dimensionality: " << dim << endl;
        exit(1);
    }
    d = dim;
    x = new double[d];
    for (int i = 0; i < d; i++)
        x[i] = 0.0;
}

vector::vector() 
{
    x = NULL;
    d = 0;
}


double &vector::operator [] (int i)
{
    if (i < 0 || i >= d) {
        cerr << endl << "vector reference of bounds: " << i << endl;
        exit (EXIT_FAILURE);
    }
    else 
        return x[i];
}


vector vector::operator * (int a)
{
    vector r(d);
    for (int i = 0; i < d; i++)
        r.x[i] = x[i] * a;
    return r;
}

vector vector::operator * (double a)
{
    vector r(d);
    for (int i = 0; i < d; i++)
        r.x[i] = x[i] * a;
    return r;
}

double vector::operator * (vector v)
{
    if (d != v.d){
        cerr << "cannot compute dot product of vectors with different dimensionality: "
             << d << " != " << v.d << endl;
        exit(1);
    }
    double sum = 0;
    for (int i = 0; i < d; i++)
        sum += x[i] * v.x[i];
    return sum;
}

vector vector::operator + (double a)
{
    vector r(d);
    for (int i = 0; i < d; i++)
        r.x[i] = x[i] + a;
    return r;
}

vector vector::operator + (vector v)
{
    vector r(d);
    if (d != v.d){
        cerr << "cannot add vectors with different dimensionality: "
             << d << " != " << v.d << endl;
        exit(1);
    }
    for (int i = 0; i < d; i++)
        r.x[i] = x[i] + v.x[i];
    return r;
}

vector vector::operator - (double a)
{
    vector r(d);
    for (int i = 0; i < d; i++)
        r.x[i] = x[i] - a;
    return r;
}

string vector::str()
{
    string ret("[");
    for (int i = 0; i < d; i++){
        ostringstream strs;
        strs << x[i];
        ret += strs.str();
        if (i < d - 1) ret += ", ";
    }
    ret += "]";
    return ret;
}


ostream& operator << (ostream& o, vector v)
{
    o << v.str();
    return o;
}
