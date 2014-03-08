// ###########################################################################
// #  File: matrix.cpp
// #  Josh Miller
// #  Homework 5
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

#include "libutilcpp/matrix.h"
#include "libutilcpp/utilcpp.h"

const char *usage = "\
Usage:\n\
    ./eigP k e filename\
";

void writeMatrix(char *path, matrix m)
{
    FILE * out = fopen(path, "w");
    fprintf(stderr, "Writing matrix to %s\n", path);
    for(int i = 0; i < m.rows; i++){
        for(int j = 0; j < m.cols; j++){
            fprintf(out, "%lf ", m[i][j]);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

void writeEigenvectors(char *path, matrix *v, int n)
{
    FILE * out = fopen(path, "w");
    fprintf(stderr, "Writing evects to %s\n", path);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < v[i].rows; j++){
            fprintf(out, "%lf ", v[i][j][0]);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

void writeEigenvalues(char *path, double *vals, int n)
{
    FILE * out = fopen(path, "w");
    fprintf(stderr, "Writing evals to %s\n", path);
    for(int i = 0; i < n; i++){
        fprintf(out, "%lf\n", vals[i]);
    }
    fclose(out);
}


int main(int argc, char *argv[])
{
    int c;
    int N = 10;
    int k = 0;
    int random = 0;
    int jacobi = 0;
    double precis = 0;

    matrix A; 

    char *input_path = NULL;
    char *eigenvectors      = (char*) "eigenvectors.out";
    char *eigenvalues       = (char*) "eigenvalues.out";
    char *eigenvaluesJacobi = (char*) "eigenvaluesJACOBI.out";

    if (argc > 0) ERROR_IF(!sscanf(argv[1], "%d", &k), "k");
    if (argc > 1) ERROR_IF(!sscanf(argv[2], "%lf", &precis), "e");
    if (argc > 2) input_path = strdup(argv[3]);

    fprintf(stderr, "k: %d\n", k);
    fprintf(stderr, "e: %e\n", precis);

    ERROR_IF(!k||!precis||!(input_path||random), "%s", usage);

    if (input_path) A = parseData(input_path);
    else            A = randomSymMatrix(N, N, 100);

    matrix *v;
    double *vals;

    v    = getEigenvectors(A, precis, k);
    vals = getEigenvalues(A, v, k);

    writeEigenvectors(eigenvectors, v, k);
    writeEigenvalues(eigenvalues, vals, k);

    vals = getEigenvaluesJacobi(A, precis, k);
    writeEigenvalues(eigenvaluesJacobi, vals, k);

    return 0;
}
