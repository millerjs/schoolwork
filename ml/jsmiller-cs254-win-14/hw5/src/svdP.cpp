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
    -k\tnumber of eigenvectors/values\n\
    -r\tN\tgenerate random matrix N -> N x N\n\
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
    int k = 0;
    int random = 0;
    double precis = 0;

    char *input_path = NULL;
    char *outputU = (char*) "svdU.out";
    char *outputV = (char*) "svdV.out";
    char *outputS = (char*) "svdS.out";
    
    opterr = 0;
    while ((c = getopt (argc, argv, "k:f:e:r")) != -1){
        if (c ==  'r'){
            random = 1;
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
    matrix A; 

    if (input_path) A = parseData(input_path);
    else{            
        A = randomSymMatrix(10, 20, 100);
        // writeMatrix((char*)"sample_svd.dat", A);
    }

    matrix *v    = getSVDvectorsV(A, precis, k);
    matrix *u    = getSVDvectorsU(A, v, k);
    double *vals = getSVDsqrtEigenvalues(A, v, k);

    matrix S = diag(vals, k);
    matrix U = matrixFromCols(u, k);
    matrix V = matrixFromCols(v, k);

    matrix Vt = ~V;
    matrix AA = U*S*Vt;

    writeMatrix(outputU, U);
    writeMatrix(outputV, V);
    writeEigenvalues(outputS, vals, k);

    cout << normFrobenius(A, AA) << endl;

    return 0;
}
