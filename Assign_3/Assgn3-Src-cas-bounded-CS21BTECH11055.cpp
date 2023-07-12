#include <iostream>
#include <random>
#include <atomic>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <sys/time.h>
using namespace std;

// parameter struct
typedef struct param
{
    double t1;
    double t2;
    FILE *fp;
    int id;
} param;

// log entry struct
typedef struct log
{
    char time[9];
} log_each;

// atomic lock
std::atomic<bool> lock{false};

log_each **log_final;
int n, iter;

// waiting array
bool *waiting;

// thread work
void *thread_work(void *p)
{
    param *pi = (param *)p;
    int id = pi->id;

    timeval curTime;
    for (int i = 0; i < iter; i++)
    {
        // request time
        gettimeofday(&curTime, NULL);

        int milli = curTime.tv_usec / 1000;
        char buffer[6];
        strftime(buffer, 6, "%M:%S", localtime(&curTime.tv_sec));
        sprintf(log_final[id][3 * i].time, "%s:%d", buffer, milli);

        // entry section
        bool expect = false;
        waiting[id] = true;
        int key = 1;

        while (waiting[id] && key == 1)
        {
            key = !lock.compare_exchange_strong(expect, true, std::memory_order_acq_rel);
            expect = false;
        }
        waiting[id] = false;

        // entry time
        gettimeofday(&curTime, NULL);
        milli = curTime.tv_usec / 1000;
        strftime(buffer, 6, "%M:%S", localtime(&curTime.tv_sec));
        sprintf(log_final[id][3 * i + 1].time, "%s:%d", buffer, milli);
        // critical section
        usleep(pi->t1 * 1000);

        int j = (id + 1) % n;
        while ((j != id) && !waiting[j])
        {
            j = (j + 1) % n;
        }

        if (j == id)
        {
            lock = false;
        }
        else
        {
            waiting[j] = false;
        }

        // exit time
        gettimeofday(&curTime, NULL);
        milli = curTime.tv_usec / 1000;
        strftime(buffer, 6, "%M:%S", localtime(&curTime.tv_sec));
        sprintf(log_final[id][3 * i + 2].time, "%s:%d", buffer, milli);

        // remainder section
        usleep(pi->t2 * 1000);
    }

    free(p);
}

int main(int argc, char const *argv[])
{

    // input scanning
    FILE *input = fopen("inp-params.txt", "r");
    int r1, r2;
    fscanf(input, "%d%d%d%d", &n, &iter, &r1, &r2);
    fclose(input);

    // exponential distribution
    double const exp_dist_mean_1 = r1;
    double const exp_dist_lamda_1 = 1 / exp_dist_mean_1;
    double const exp_dist_mean_2 = r2;
    double const exp_dist_lamda_2 = 1 / exp_dist_mean_2;
    int seed = chrono::system_clock::now().time_since_epoch().count();

    default_random_engine gen(seed);

    exponential_distribution<double> rng_1(exp_dist_lamda_1);
    exponential_distribution<double> rng_2(exp_dist_lamda_2);

    // output file
    FILE *fp;
    fp = fopen("output_BWCAS.txt", "w");

    // allocate memory for thread_id's and log
    pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * n);
    waiting = (bool *)malloc(sizeof(bool) * n);
    log_final = (log_each **)malloc(sizeof(log_each *) * n);

    for (int i = 0; i < n; i++)
    {
        log_final[i] = (log_each *)malloc(sizeof(log_each) * (3 * iter));
    }

    // create thread and assign task
    for (int i = 0; i < n; i++)
    {
        param *p = (param *)malloc(sizeof(param));
        p->fp = fp;
        p->t1 = rng_1(gen);
        p->t2 = rng_2(gen);
        p->id = i;
        pthread_create(&tid[i], NULL, thread_work, (void *)p);
    }

    // wait for all threads completion by main thread
    for (int i = 0; i < n; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // log generation
    fprintf(fp, "Bounded Waiting CAS ME Output(format:min/sec/millisec):\n\n");

    // sorting log
    for (int w = 0; w < iter; w++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int it = 3 * w; it < 3 * w + 3; it++)
            {
                auto i = log_final[j][it];
                if (it % 3 == 0)
                {
                    fprintf(fp, "%dth CS Requested at %s by thread  %d \n", w, i.time, j);
                }
                else if (it % 3 == 1)
                {
                    fprintf(fp, "%dth CS Entered at %s by thread  %d \n", w, i.time, j);
                }
                else
                {
                    fprintf(fp, "%dth CS Exited at %s by thread  %d \n", w, i.time, j);
                }
            }
        }
        fprintf(fp, "\n");
    }

    // free all allocated memory
    fclose(fp);
    free(tid);
    free(waiting);

    for (int i = 0; i < n; i++)
    {
        free(log_final[i]);
    }

    free(log_final);
    return 0;
}
