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
#include "utilcpp.h"

double **_new_2d_array(int r, int c, double init)
{
    double ** x = new double*[r];
    for(int i = 0; i < r; i++){
        x[i] = new double[c];
        std::fill(x[i], x[i]+c, init);
    }
    return x;
}

matrix::matrix(const int _m, const int _n, double init) 
{

    ERROR_IF(_m <= 0 || _n <= 0, "matrix cannot have non-positive dimensionality: %dx%d", _m, _n);
    rows = _m;
    cols = _n;
    x = _new_2d_array(rows, cols, init);
}

matrix::matrix(const int _m, const int _n) 
{
    ERROR_IF(_m <= 0 || _n <= 0, "matrix cannot have non-positive dimensionality: %dx%d", _m, _n);
    rows = _m;
    cols = _n;
    x = _new_2d_array(rows, cols, 0.0);
}

matrix::matrix() 
{
    rows = cols = 0;
    x = NULL;
}

matrix::~matrix() 
{
    if (rows && cols){
        for (int i = 0; i < rows; i++){
            delete [] x[i];
            x[i] = NULL;
        }
        delete [] x;
    }
    rows = 0;
    cols = 0;
}

matrix& matrix::operator= (const matrix& other){
    if (this != &other) {
        rows = other.rows;
        cols = other.cols;
        x = _new_2d_array(other.rows, other.cols, 0.0);
        for (int i = 0; i < other.rows; i++){
            memcpy(x[i], other.x[i], cols*sizeof(double));
        }
    }
    return *this;
}

matrix::matrix(const matrix& other){
    rows = other.rows;
    cols = other.cols;
    x = _new_2d_array(other.rows, other.cols, 0.0);
    for (int i = 0; i < other.rows; i++){
        memcpy(x[i], other.x[i], cols*sizeof(double));
    }
}

matrix matrix::copy() 
{
    matrix a(rows, cols);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            a[i][j] = x[i][j];
        }
    }
    return a;
}

double *&matrix::operator [] (int i)
{
    assert(i >= 0 && i < rows);
    return x[i];
}

matrix matrix::operator * (int a)
{
    matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ret[i][j] = x[i][j] * a;
    return ret;
}

matrix matrix::operator ~()
{
    matrix ret(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ret[j][i] = x[i][j];
    return ret;
}

matrix matrix::operator * (double a)
{
    matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ret[i][j] = x[i][j] * a;
    return ret;
}

matrix matrix::operator * (matrix& a)
{
    assert(a.rows == cols);
    matrix ret(rows, a.cols, 0.0);
    for(int i = 0; i < ret.rows; i++){
        for(int j = 0; j < ret.cols; j++){
            for(int k = 0; k < cols; k++){
                ret[i][j] += x[i][k] * a[k][j];
            }
        }
    }

    return ret;
}

matrix matrix::operator + (double a)
{
    matrix ret(rows, cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            ret[i][j] = x[i][j] + a;

        }
    }
    return ret;
}

matrix matrix::operator += (double a)
{
    matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ret[i][j] = x[i][j] + a;
    return ret;
}

matrix matrix::operator + (matrix& v)
{
    matrix r(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r[i][j] = x[i][j] + v[i][j];
    return r;
}

matrix matrix::operator - (double a)
{
    matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ret[i][j] = x[i][j] - a;
    return ret;
}

matrix matrix::operator - (matrix& v)
{
    matrix r(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r[i][j] = x[i][j] - v[i][j];
    return r;
}


double sum(matrix& m)
{
    double s = 0;
    for (int i = 0; i < m.rows; i++){
        for(int j = 0; j < m.cols; j++){
            s += m[i][j];
        }
    }
    return s;
}

void matrix::normalize()
{
    double sum = 0;
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            sum += x[i][j];
        }
    }

    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            x[i][j] /= sum;
        }
    }

    return;
}

double matrix::product()
{

    double prod = 0;
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            prod *= x[i][j];
        }
    }
    return prod;
}

void matrix::abs()
{
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            x[i][j] = std::abs(x[i][j]);
        }
    }
    return;
}

ostream& operator << (ostream& o, matrix v)
{
    o <<  "[";
    for (int i = 0; i < v.rows; i++){
        o <<"[";
        for (int j = 0; j < v.cols; j++){
            ostringstream strs;
            strs << v[i][j];
            o <<strs.str();
            if (j < v.cols - 1) o <<", ";
        }    
        o <<"]";
        if (i < v.rows - 1) o <<",\n ";
    }
    o <<"]";
    return o;
}


matrix matrix::operator()(const int r, const int c)
{
    assert(r < rows);
    assert(c < cols);

    if (r < 0 && c < 0){
        return this->copy();

    } else if (c < 0){
        matrix ret(1, cols);
        for(int i = 0; i < cols; i++){
            ret[0][i] = x[r][i];
        }
        return ret;

    }  else if (r < 0) {

        matrix ret(rows, 1);
        for(int i = 0; i < rows; i++){
            ret[i][0] = x[i][c];
        }
        return ret;

    } 
        
    matrix ret(1, 1, x[r][c]);
    return ret;

}
