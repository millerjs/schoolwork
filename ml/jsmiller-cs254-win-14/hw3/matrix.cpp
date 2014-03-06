// ###########################################################################
// #  File: matrix.cpp
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
#include <iomanip>
#include <cmath>

#include "matrix.h"
#include "util.h"

matrix::matrix(const int _m, const int _n, double init) 
{

    ERROR_IF(_m <= 0 || _n <= 0, "matrix cannot have non-positive dimensionality: %dx%d", m, n);
    m = _m;
    n = _n;
    x = new double*[m];
    for(int i = 0; i < m; i++){
        x[i] = new double[n];
        std::fill(x[i], x[i]+n, init);
    }
}

matrix::matrix(const int _m, const int _n) 
{
    ERROR_IF(_m <= 0 || _n <= 0, "matrix cannot have non-positive dimensionality: %dx%d", m, n);
    m = _m;
    n = _n;    
    x = new double*[m];
    for(int i = 0; i < m; i++){
        x[i] = new double[n];
        std::fill(x[i], x[i]+n, 0.0);
    }
}

matrix::matrix() 
{
    m = n = 0;
    x = NULL;
}

matrix::~matrix() 
{
    // if (x) delete [] x;
}

matrix matrix::copy() 
{
    matrix a(m, n);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            a[i][j] = x[i][j];
        }
    }
    return a;
}

double *&matrix::operator [] (int i)
{
    if (i < 0 || i >= m) {
        cerr << "matrix reference out of bounds: " << i << endl;
        exit (EXIT_FAILURE);
    }
    else return x[i];
}

matrix matrix::operator * (int a)
{
    matrix r(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            r[i][j] = x[i][j] * a;
    return r;
}

matrix matrix::operator ~()
{
    matrix r(n, m);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            r[j][i] = x[i][j];
    return r;
}

matrix matrix::operator * (double a)
{
    matrix r(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            r[i][j] = x[i][j] * a;
    return r;
}

matrix matrix::operator * (matrix a)
{
    ERROR_IF( n!= a.m, "Dim mismatch: %d != %d", n, m);
    matrix newm(m, a.n, 0.0);
    for(int i = 0; i < m; i++)                        
        for(int j = 0; j < n; j++)                    
            for(int k = 0; k < n; k++)               
                newm[i][j] += x[i][k]*a[k][j];
    return newm;
}

matrix matrix::operator + (double a)
{
    matrix r(m, n, 0.0);
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            r[i][j] = x[i][j] + a;

        }
    }
    return r;
}

matrix matrix::operator += (double a)
{
    matrix r(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            r[i][j] = x[i][j] + a;
    return r;
}

matrix matrix::operator + (matrix v)
{
    matrix r(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            r[i][j] = x[i][j] + v[i][j];
    return r;
}

matrix matrix::operator - (double a)
{
    matrix r(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            r[i][j] = x[i][j] - a;
    return r;
}

matrix matrix::operator - (matrix v)
{
    matrix r(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            r[i][j] = x[i][j] - v[i][j];
    return r;
}

string matrix::str()
{
    string ret("[");
    for (int i = 0; i < m; i++){
        ret += "[";
        for (int j = 0; j < n; j++){
            ostringstream strs;
            // strs << std::setprecision(4);
            strs << x[i][j];
            ret += strs.str();
            if (j < n - 1) ret += ", ";
        }    
        ret += "]";
        if (i < m - 1) ret += ",\n ";
    }
    ret += "]";
    return ret;
}

double matrix::sum()
{

    double s = 0;
    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            s += x[i][j];
        }
    }
    return s;
}

void matrix::normalize()
{

    double sum = 0;
    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            sum += x[i][j];
        }
    }

    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            x[i][j] /= sum;
        }
    }

    return;
}

double matrix::product()
{

    double prod = 0;
    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            prod *= x[i][j];
        }
    }
    return prod;
}

void matrix::abs()
{
    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            x[i][j] = std::abs(x[i][j]);
        }
    }
    return;
}

matrix matrix::operator /(int a)
{
    matrix r(1, n);
    for(int i = 0; i < n; i++){
        r[0][i] = x[a][i];
    }
    return r;
}

matrix matrix::operator %(int a)
{
    matrix r(m, 1);
    for(int i = 0; i < m; i++){
        r[i][0] = x[i][a];
    }
    return r;
}


ostream& operator << (ostream& o, matrix v)
{
    o << v.str();
    return o;
}

