#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <time.h>
#define BILLION 1000000000.0

int N;      // Size of sudoku
int K;      // No of thread
int **mat;  // sukodu 2D array
int *valid; // validation variable
// parameter for thread
typedef struct params
{
    int start;
    int size;
} params;

/*Validation of row*/
int row_check(int j)
{
    bool *check = calloc(N, sizeof(bool));


     /*Check for repetition*/
    for (int i = 0; i < N; i++)
    {
        int temp = mat[j][i] - 1;
        if (check[temp] == false)
        {
            check[temp] = true;
        }
        else
        {
            /*return false if there is repetition*/
            free(check);
            return 0;
        }
    }
    free(check);
    return 1;
}

/*Validation of column*/
int col_check(int j)
{
    bool *check = calloc(N, sizeof(bool));


         /*Check for repetition*/
    for (int i = 0; i < N; i++)
    {
        int temp = mat[i][j] - 1;
        if (check[temp] == false)
        {
            check[temp] = true;
        }
        else
        {
            /*return false if there is repetition*/
            free(check);
            return 0;
        }
    }
    free(check);
    return 1;
}

/*Validation of sub grid*/
int grid_check(int j)
{
    int sp = sqrt((double)N);
    bool *check = calloc(N, sizeof(bool));



             /*Check for repetition*/
    for (int i = (j % sp) * sp; i < (j % sp) * sp + sp; i++)
    {
        for (int k = (j / sp) * sp; k < (j / sp) * sp + sp; k++)
        {
            int temp = mat[i][k] - 1;
            if (check[temp] == false)
            {
                check[temp] = true;
            }
            else
            {
            /*return false if there is repetition*/
                free(check);
                return 0;
            }
        }
    }
    free(check);
    return 1;
}


/*Function executed by thread*/
void thread_work(params p)
{

    for (int i = p.start; i < p.start + p.size; i++)
    {
        if (i < N)
        {
            valid[i] = row_check(i);//invoke row validation 
        }
        else if (N <= i && i < 2 * N)
        {
            valid[i] = col_check(i - N);//invoke column validation
        }
        else
        {
            valid[i] = grid_check(i - 2 * N);//invoke sub grid validation
        }
    }
}

/*Log generation*/
void log_gen(FILE *fp, int start, int size,int index)
{
    for (int i = start; i < start + size; i++)
    {
        /*Check the valid variable and print the log*/
        if (i < N)
        {
            fprintf(fp, "Thread%d checks ROW %d: %s \n",index,i, (valid[i]) ? "valid" : "invalid");
        }
        else if (N <= i && i < 2 * N)
        {
            fprintf(fp, "Thread%d checks COL %d: %s \n",index,i - N, (valid[i]) ? "valid" : "invalid");
        }
        else
        {
            fprintf(fp, "Thread%d checks GRID %d: %s \n",index,i - 2 * N, (valid[i]) ? "valid" : "invalid");
        }
    }
}


/*Main function*/
int main(int argc, char const *argv[])
{
    /*Timer variables*/
    struct timespec start_clock, end;
    clock_gettime(0, &start_clock);
   

   /*Taking input*/
    FILE *ip = fopen("input.txt", "r");
    fscanf(ip, "%d", &K);//scan K
    fscanf(ip, "%d", &N);//scan N


   /*Scan sudoku matrix/2D array*/
    mat = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        mat[i] = (int *)malloc(N * sizeof(int));
    }

    valid = malloc(sizeof(int) * (3 * N));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fscanf(ip, "%d", &mat[i][j]);
        }
    }
    fclose(ip);



    int start = 0;
    /*Set the number of threads*/
    omp_set_num_threads(K);
    params p;

    /*Convert sequential code and divide among threads*/
    #pragma omp parallel
    {
        int i = omp_get_thread_num() + 1;
        if (i <= (3 * N) % K)
        {
            p.start = start;
            p.size = (3 * N) / K + 1;
            start = start + p.size;
        }
        else
        {
            p.start = start;
            p.size = (3 * N) / K;
            start = start + p.size;
        }
        thread_work(p);
    }

    /*Output Generation*/

    FILE *fp = fopen("output.txt", "w");

    int check = 1;
    for (int i = 0; i < 3 * N; i++)
    {
        if (valid[i] == 0)
        {
            check = 0;
            break;
        }
    }
    fprintf(fp, "Open mp\n\n");
    if (check)
    {
        fprintf(fp, "valid sudoku\n\n");
    }
    else
    {
        fprintf(fp, "invalid sudoku\n\n");
    }
    clock_gettime(0, &end);

   /*print time taken*/
    double time_spent = (end.tv_sec - start_clock.tv_sec) +
                        (end.tv_nsec - start_clock.tv_nsec) / BILLION;

    fprintf(fp, "The elapsed time is %f seconds\n", time_spent);
    fprintf(fp, "Log:\n\n");

   /*Log generation*/
    start = 0;
    for (int i = 1; i <= K; i++)
    {
        fprintf(fp, "Thread %d:\n", i);
        if (i <= (3 * N) % K)
        {
            log_gen(fp, start, (3 * N) / K + 1,i);
            start = start + (3 * N) / K + 1;
        }
        else
        {
            log_gen(fp, start, (3 * N) / K,i);
            start = start + (3 * N) / K;
        }
        fprintf(fp, "\n\n");
    }
    fclose(fp);


    /*Free allocated memory*/
    for (int i = 0; i < N; i++)
    {
        free(mat[i]);
    }

    free(mat);
    free(valid);
    return 0;
}
