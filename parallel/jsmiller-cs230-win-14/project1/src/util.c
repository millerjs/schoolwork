/******************************************************************************/
/*                                                                            */
/* FILE    : util.h                                                           */
/* AUTHOR  : Joshua Miller                                                    */
/* PROJECT : Project 1                                                        */
/* CLASS   : Parallel Computing - Winter 2014                                 */
/*                                                                            */
/******************************************************************************/



#include "util.h"

void error(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "\nERROR: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}



int *fread_adjacency(char *fin, int *n)
{

    int i;
    size_t rs = MAX_BUFF_LEN;
    FILE *fp;
    char *buffer;
    int *m;
    int N = 0;

    if (!(buffer = (char *) malloc(MAX_BUFF_LEN)))
        error("unable to allocate buffer");

    if (!(fp = fopen(fin, "r")))
        error("unable to open input file [%s]", fin);

    if (fscanf(fp, "%d\n", &N) != 1)
        error("unable to parse matrix size");

    if (!(m = malloc(N * N * sizeof(int))))
        error("unable to allocate memory");
    
    char *token;
    int curs = 0;
    int j;
    for (i = 0; i < N; i++) {

        rs = MAX_BUFF_LEN;

        if (getline(&buffer, &rs, fp) < 0)
            error("bad file read");

        token = strtok(buffer, " ");

        for (j = 0; j < N; j++){
            m[curs++] = atoi(token);
            token = strtok(NULL, " ");
        }


    }

    *n = N;

    return m;

}


int fprint_matrix(FILE * f, int *m, int n)
{

    int i, j;
    for (i = 0; i < n; i++) {
        fprintf(f, " |");
        for (j = 0; j < n; j++) {
            fprintf(f, "%3d ", m[i _ j]);

        }
        fprintf(f, "|\n");
    }
    fprintf(f, "\n");

    return 0;

}


int *copy_matrix(int *m, int n)
{

    int *new = (int *) malloc(n * n * sizeof(int));

    if (!new)
        error("unable to allocate for matrix copy");

    memcpy(new, m, n*n*sizeof(int));

    return new;

}

int *random_matrix(int n)
{

    int i, j;
    int *new = (int *) malloc(n*n*sizeof(int));

    srand(time(NULL));

    if (!new)
        error("unable to allocate for random matrix");

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != j)
                new[i _ j] = (int) rand() % 20;
            else
                new[i _ j] = 0;
        }
    }


    return new;

}

int compare_matrices(int *m1, int *m2, int n)
{

    int i, j;

    int err = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (m1[i _ j] != m2[i _ j]) {
                err++;
                fprintf(stderr, "Mismatch at [%d, %d]\n", i, j);
            }
        }
    }

    if (err > 0)
        fprintf(stderr, "There were [ %d ] discrepencies.\n", err);

    return err;

}
