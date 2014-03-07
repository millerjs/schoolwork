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

/* Pull ASCII representation (w/ whitespace) into matrix */
matrix parseData(const char *path);

/* Create a random matrix with 0 < A_ij < max */
matrix randomMatrix(int rows, int cols, double max);

/* Create a random symmetric matrix with 0 < A_ij < max */
matrix randomSymMatrix(int rows, int cols, double max);

/* Get's the eigenvalues of a matrix given array of eigenvectors */
double *getEigenvalues(matrix& A, matrix *v, int n);

/* Get's the eigenvalues of a matrix given array of eigenvectors */
double *getSVDsqrtEigenvalues(matrix& A, matrix *v, int n);

/* Get's the eigenvalues of a SQUARE SYMMETRIC MATRIX */
matrix *getEigenvectors(matrix& A, double precis, int order);

/* Get's the SVD eigenvalues v_i of matrix A */
matrix *getSVDvectorsV(matrix& A, double precis, int order);

/* Get's the SVD eigenvalues u_i of matrix A given v_i */
matrix *getSVDvectorsU(matrix& A, matrix *v, int n);

/* Constructs matrix from column vectors */
matrix matrixFromCols(matrix *cols, int n);

/* Constructs a square diagnonal matrix from double array vals */
matrix diag(double *vals, int n);

/* Calculates the frobenius norm between two matrices */
double normFrobenius(matrix& A, matrix& B);

/* Generates a random vector in the unit sphere of length n*/
matrix *randomSphereVectors(int dimensions, int n);

/* Returns a deep copy of the data in matrix m */
double ** matrixToDouble(matrix& m);

#endif

