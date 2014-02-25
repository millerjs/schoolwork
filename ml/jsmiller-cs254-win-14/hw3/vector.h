// ###########################################################################
// #  File: hw3.cpp
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

#ifndef _VECTOR_H
#define _VECTOR_H

class vector
{
public:
    int d; 
    double *x;

    /* constructors */
    vector(void);
    vector(int dim);
    vector(int dim, double init);
    double &operator [] (int i);
    
    /* multiplication */
    vector operator * (int a);
    vector operator * (double a);
    double operator * (vector v1);
    
    /* addition */
    vector operator + (double a);
    vector operator + (vector v);
    
    /* subtraction */
    vector operator - (double a);
    vector operator - (vector v);

    /* conversion */
    string str();

};

ostream& operator << (ostream& o, vector v);

#endif

