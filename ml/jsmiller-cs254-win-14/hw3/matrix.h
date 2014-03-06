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

#include "vector.h"

#ifndef _MATRIX_H
#define _MATRIX_H

class matrix
{
public:
    int m;
    int n; 
    double **x;

    /* constructors */

    matrix();
    matrix copy();
    matrix(int m, int n);
    matrix(int m, int n, double init);
    ~matrix();

    double *&operator [] (int i);
    
    /* multiplication */
    matrix operator * (int a);
    matrix operator * (double a);
    matrix operator * (matrix v1);
    double product();

    /* addition */
    matrix operator + (double a);
    matrix operator + (matrix v);
    matrix operator += (double a);
    
    /* subtraction */
    matrix operator - (double a);
    matrix operator - (matrix v);

    /* transpose */
    matrix operator ~();

    /* conversion */
    string str();
    
    /* slice m[a, :] */
    matrix operator /(int a);

    /* slice m[:, a] */
    matrix operator %(int a);

    /* Other */
    void normalize();
    double sum();
    void abs();

};

ostream& operator << (ostream& o, matrix v);

#endif

