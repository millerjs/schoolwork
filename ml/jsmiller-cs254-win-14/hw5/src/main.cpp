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

#include "libutilcpp/matrix.h"
#include "libutilcpp/utilcpp.h"

const char *usage = "\
Usage:\n\
    -n\tnumber of eigenvalues to find\n\
    -f\tinput matrix file\n\
    -e\tprecision\
";

int main(int argc, char *argv[])
{

    int c;
    int nvals = 0;
    double precis = 0;
    char *input_path = NULL;
    
    opterr = 0;
     
    while ((c = getopt (argc, argv, "n:f:e:")) != -1){
        switch (c) {
        case 'n':
            ERROR_IF(!sscanf(optarg, "%d", &nvals), "parsing -n");
            break;
        case 'f':
            input_path = strdup(optarg);
            break;
        case 'e':
            ERROR_IF(!sscanf(optarg, "%lf", &precis), "parsing -e");
            break;
        default:
            ERROR("unknown option: %c", c);
        } 
    }

    ERROR_IF(!nvals||!precis||!input_path, "%s", usage);


    return 0;
}
