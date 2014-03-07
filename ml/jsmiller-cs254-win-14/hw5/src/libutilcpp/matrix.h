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
#include <cassert> 

#ifndef _MATRIX_H
#define _MATRIX_H

#define _ -1

class matrix
{
public:
    int rows;
    int cols; 

    double **x;

    /* constructors */

    matrix operator()(const int r, const int c);

    matrix& operator= (const matrix& other);
    matrix(const matrix& other);

    matrix();
    matrix copy();
    matrix(int m, int n);
    matrix(int m, int n, double init);
    ~matrix();

    double *&operator [] (int i);
    
    /* multiplication */
    matrix operator * (int a);
    matrix operator * (double a);
    matrix operator * (matrix& v1);
    double product();

    matrix operator / (double a);

    /* addition */
    matrix operator + (double a);
    matrix operator + (matrix& v);
    matrix operator += (double a);
    
    /* subtraction */
    matrix operator - (double a);
    matrix operator - (matrix& v);

    /* transpose */
    matrix operator ~();

    /* Other */
    void normalize();
    void abs();
    double dot(matrix& v2);
    void orthogonalize(matrix *v, int n);

};

ostream& operator << (ostream& o, matrix v);
double sum(matrix& m);
double norm(matrix m);
matrix parseData(const char *path);
matrix randomMatrix(int rows, int cols, double max);
matrix randomSymMatrix(int rows, int cols, double max);
double getEigenvalue(matrix& m, matrix& v, double precis);
matrix *getEigenvectors(matrix& A, double precis, int order);

#endif

