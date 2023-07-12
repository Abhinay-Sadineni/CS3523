#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

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
void *thread_work(void *par)
{
    params *p = (params *)par;


    for (int i = p->start; i < p->start + p->size; i++)
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
            valid[i] = grid_check(i - 2 * N);//invoke subgrid validation 
        }
    }
    free(par);
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
    fscanf(ip, "%d", &K);
    fscanf(ip, "%d", &N);

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


    /*Setting number of threads*/
    pthread_t *tid = malloc(sizeof(pthread_t) * K);
    int start = 0;

    /*Convert sequential code and divide among threads*/
    for (int i = 1; i <= K; i++)
    {
        params *p = malloc(sizeof(params));
        if (i <= (3 * N) % K)
        {
            p->start = start;
            p->size = (3 * N) / K + 1;
            start = start + p->size;
        }
        else
        {
            p->start = start;
            p->size = (3 * N) / K;
            start = start + p->size;
        }
        pthread_create(&tid[i - 1], NULL, thread_work, (void *)p);
    }

    
    for (int i = 0; i < K; i++)
    {
        pthread_join(tid[i], NULL);
    }


     /*Output generation*/
    FILE *fp = fopen("output.txt", "w");
    fprintf(fp,"Pthread\n\n");

    int check = 1;
    for (int i = 0; i < 3 * N; i++)
    {
        if (valid[i] == 0)
        {
            check = 0;
            break;
        }
    }
    if (check)
    {
        fprintf(fp, "valid sudoku\n\n");
    }
    else
    {
        fprintf(fp, "invalid sudoku\n\n");
    }

    clock_gettime(0, &end);

    /*Print time taken*/
    double time_spent = (end.tv_sec - start_clock.tv_sec) * 1000000 +
                        (end.tv_nsec - start_clock.tv_nsec) / 1000.0;

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
    free(tid);

    for (int i = 0; i < N; i++)
    {
        free(mat[i]);
    }

    free(mat);
    free(valid);
    return 0;
}
