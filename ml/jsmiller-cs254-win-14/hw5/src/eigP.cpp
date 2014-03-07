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
    -f\tinput matrix file\n\
    -v\tnumber of eigenvectors/values\n\
    -k\tN\tgenerate random matrix N -> N x N\n\
    -e\tprecision\
";

void writeMatrix(char *path, matrix m)
{
    FILE * out = fopen(path, "w");
    fprintf(stdout, "Writing matrix to %s\n", path);
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
    fprintf(stdout, "Writing evects to %s\n", path);
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
    fprintf(stdout, "Writing evals to %s\n", path);
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
    double precis = 0;

    matrix A; 

    char *input_path = NULL;
    char *eigenvectors = (char*) "eigenvectors.out";
    char *eigenvalues  = (char*) "eigenvalues.out";

    opterr = 0;
    while ((c = getopt (argc, argv, "k:f:e:r:")) != -1){
        if (c ==  'r'){
            random = 1;
            ERROR_IF(!sscanf(optarg, "%d", &N), "parsing -r");
        } else if (c == 'k'){
            ERROR_IF(!sscanf(optarg, "%d", &k), "parsing -v");
        } else if (c == 'f'){
            input_path = strdup(optarg);
        } else if (c == 'e'){
            ERROR_IF(!sscanf(optarg, "%lf", &precis), "parsing -e");
        } else
            ERROR("unknown option: %c", c);
    }

    ERROR_IF(!k||!precis||!(input_path||random), "%s", usage);

    if (input_path) A = parseData(input_path);
    else            A = randomSymMatrix(N, N, 100);

    matrix *v = getEigenvectors(A, precis, k);
    double *vals = getEigenvalues(A, v, k);

    writeEigenvectors(eigenvectors, v, k);
    writeEigenvalues(eigenvalues, vals, k);

    return 0;
}
