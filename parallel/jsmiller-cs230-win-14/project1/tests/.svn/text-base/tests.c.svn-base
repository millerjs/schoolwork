/******************************************************************************/
/*                                                                            */
/* FILE    : tests.h                                                          */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 1                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"
#include "../src/floyd.h"
#include "../src/util.h"
#include "../src/stopwatch.h"

#define GREEN "\033[0;32m"
#define NORM "\033[0m"
#define RED "\033[0;31m"

int sample1[] = {0, 1, 10000000, 1, 5, 
                 9, 0, 3, 2, 10000000, 
                 10000000, 10000000, 0, 4, 10000000, 
                 10000000, 10000000, 2, 0, 3, 
                 3, 10000000, 10000000, 10000000, 0};

void result(int failed, char* test, char* subtest){

    if (failed){
        printf("TEST: [%30s] [%30s] %s \tFAILED %s\n", test, subtest, RED, NORM); 
        exit(1);
    } 

    printf("TEST: [%30s] [%30s] %s \tPASSED %s\n", test, subtest, GREEN, NORM); 


}

int test_input(char *path, int **matrix, int n){
    
    int n_in = 0;

    *matrix = fread_adjacency(path, &n_in);

    int failed = compare_matrices(*matrix, sample1, n) || n - n_in;

    result(failed, "test_input", path);

    return 0;

}


int test_copy(int *matrix, int n){
    
    int * m2 = copy_matrix(matrix, n);
    int failed = compare_matrices(m2, matrix, n);
    result(failed, "test_copy", "");
    return 0;
 
}

int test_serial(char *path, int *m){

    int n_in;
    int *correct = fread_adjacency(path, &n_in);
    int *test = copy_matrix(m, n);

    floyd_serial(test);
    
    int failed = compare_matrices(correct, test, n);

    result(failed, "test_serial", path);

    return 0;

}

int test_parallel(char *path, int *m){

    int n_in;
    int *correct = fread_adjacency(path, &n_in);
    int *test;

    int i, failed = 0;

    int n_repeat = 200;
    
    for ( i = 0; i < n_repeat; i ++){
        
        test = copy_matrix(m, n);
        
        floyd_parallel(test);
        
        if ((failed = compare_matrices(correct, test, n)))
            break;

    }

    result(failed, "test_parallel x 200", path);

    return 0;

}

int test_random_parallel(int w){

    n = w;

    int *original = random_matrix(n);
    int *serial = copy_matrix(original, n);

    floyd_serial(serial);

    int i, failed = 0;
    int n_repeat = 200;
    
    for ( i = 0; i < n_repeat; i ++){
        
        int *parallel = copy_matrix(serial, n);
        
        floyd_parallel(parallel);
        
        if ((failed = compare_matrices(serial, parallel, n)))
            break;

    }

    char descrip[128];
    sprintf(descrip, "n = %d, n_threads = %d", w, n_threads);

    result(failed, "random parallel x 200", descrip);
    
    return failed;

}


int main(int argc, char*argv[])
{

    /* Test read in  */
    
    int * m = NULL; 
    test_input("tests/sample1.mtrx", &m, 5);

    /* Test copy */
    test_copy(m, 5);

    n_threads = 4;

    n = 5;
    test_serial("tests/output1.mtrx", m);
    test_parallel("tests/output1.mtrx", m);
    
    m = fread_adjacency("tests/sample3.mtrx", &n);
    test_serial("tests/output3.mtrx", m);
    test_parallel("tests/output3.mtrx", m);
    
    n_threads = 4;
    test_random_parallel(9);
    test_random_parallel(10);
    test_random_parallel(43);

    n_threads = 11;
    test_random_parallel(9);
    test_random_parallel(10);
    test_random_parallel(43);


}

