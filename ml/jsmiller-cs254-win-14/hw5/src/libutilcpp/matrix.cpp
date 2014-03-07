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

double matrix::dot(matrix& v1)
{
    double ret = 0;
    assert(rows == v1.rows && cols == v1.cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ret += x[i][j]*v1[i][j];
    return ret;
}

matrix matrix::operator / (double a)
{
    matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ret[i][j] = x[i][j] / a;
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

double norm(matrix m)
{
    double sum = 0;
    if (m.rows == 1){
        for (int i = 0; i < m.cols; i++)
            sum += m[0][i]*m[0][i];
    } else if (m.cols == 1){
        for (int i = 0; i < m.rows; i++)
            sum += m[i][0]*m[i][0];
    } else {
        WARN("cannot compute norm of non-vector matrix");
        return -1;
    }
    return pow(sum, .5);
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

matrix parseData(const char *path)
{
    FILE *input = fopen(path, "r");
    ERROR_IF(!input, "unable to open file: %s", path);

    int N = 0;
    while (EOF != (fscanf(input, "%*[^\n]"), fscanf(input, "%*c"))) N++; 
    fprintf(stderr, "Parsing Data: %d x %d\n", N, N); 
    ERROR_IF(!N, "File data must have non-zero dimensions"); 
    fseek(input, 0, SEEK_SET);     

    matrix ret(N, N);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            ERROR_IF(!fscanf(input, "%lf", &ret[i][j]), "element read"); 
        }        
    }

    return ret;
}

double ** matrixToDouble(matrix& m)
{
    double ** ret = _new_2d_array(m.rows, m.cols, 0.0);
    for(int i = 0; i < m.rows; i++){
        for(int j = 0; j < m.cols; j++){
            ret[i][j] = m[i][j];
        }
    }
    return ret;
}

matrix randomMatrix(int rows, int cols, double max)
{
    matrix ret(rows, cols);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            ret[i][j] = rand()*max/RAND_MAX;
        }
    }    
    return ret;
}


matrix *randomSphereVectors(int dimensions, int n)
{
    matrix *ret = new matrix[n];
    for(int i = 0; i < n; i++){
        ret[i] = randomMatrix(dimensions, 1, 1);
        ret[i].normalize();
    }
    return ret;
}


matrix randomSymMatrix(int rows, int cols, double max)
{
    matrix ret(rows, cols);
    for(int i = 0; i < rows; i++){
        for(int j = i; j < cols; j++){
            ret[i][j] = rand()*max/RAND_MAX;
        }
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < i; j++){
            ret[i][j] = ret[j][i];
        }
    }    
    return ret;
}

double *getEigenvalues(matrix& A, matrix *v, int n)
{
    double *vals = new double[n];
    for(int i = 0; i < n; i++){
        matrix v2;
        if (v[i].rows == 1){
            matrix v3 = ~v[i];
            v2 = A*v3;
        } else if (v[i].cols == 1){
            v2 = A*v[i];
        } else {
            WARN("bad eigenvector dimensions");
            return 0;
        }
        double sum = 0;
        vals[i] = v2[0][0]/v[i][0][0];
        matrix tmp = A*v[i];
        matrix tmp2 = v[i]*vals[i];
        double error = normFrobenius(tmp, tmp2);
        fprintf(stderr, "eval[%.3e]:\t(||A*v_i - v[i]*val[i])||_F = %lf\n", vals[i], error);
    }
    return vals;
}


double *getSVDsqrtEigenvalues(matrix& A, matrix *v, int n)
{
    matrix AA = (~A) * A;
    double *vals = getEigenvalues(AA, v, n);
    for(int i = 0; i < n; i++)
        vals[i] = pow(vals[i], .5);
    return vals;
}

void matrix::orthogonalize(matrix *v, int n)
{
    for(int i = 0; i < n; i++){
        matrix t1 = v[i] * (v[i].dot( (*this) ));
        (*this) = (*this) - t1;
    }
}

matrix *getEigenvectors(matrix& A, double precis, int order)
{
    matrix *ret = new matrix[order];
    matrix r, r2, epos, eneg;
    for(int i = 0; i < order; i++){
        cerr << "Calculating v_" << i << endl;
        r = randomMatrix(A.cols, 1, 20.0);
        int j = 0;
        do {
            r.orthogonalize(ret, i);
            r2 = A*r / norm(A*r);
            epos = r2 - r;
            eneg = r2 + r;
            r = r2;
            if (j++ > 100000) break;
        } while ( norm(epos) > precis && norm(eneg) > precis);
        // fprintf(stdout, "%d\t%lf\t%d\n", A.rows, log10(precis), j);
        ret[i] = r;
    }
    return ret;
}

matrix *getSVDvectorsV(matrix& A, double precis, int k)
{
    matrix AA = (~A) * A;
    matrix *v = getEigenvectors(AA, precis, k);
    return v;
}

matrix matrixFromCols(matrix *cols, int n)
{
    int rows = cols[0].rows;
    matrix ret(rows, n);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < n; j++){
            ret[i][j] = cols[j][i][0];
        }
    }
    return ret;
}

matrix *getSVDvectorsU(matrix& A, matrix *v, int n)
{
    matrix *u = new matrix[n];
    for(int i = 0; i < n; i++){
        u[i] = (A * v[i]) / norm (A * v[i]);
    }
    return u;
}


matrix diag(double *vals, int n)
{
    matrix ret(n, n);
    for(int i = 0; i < n; i++)
        ret[i][i] = vals[i];
    return ret;
}

double normFrobenius(matrix& A, matrix& B)
{
    double ret = 0;
    for(int i = 0; i < A.rows; i++){
        for(int j = 0; j < A.cols; j++){
            ret += pow(A[i][j] - B[i][j], 2.0);
        }
    }
    return pow(ret, .5);
}



