#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>
#include <chrono>
#include <random>
using namespace std;

// parameter struct
typedef struct param
{
    double T;
    FILE *fp;
    int id;
} param;

sem_t *car_fill_seat;  // semaphore for vacent seat
sem_t *ride_over;      // semaphore for finish ride
sem_t *car_seat_taken; // semaphore for filled seat
sem_t passenger;       // semaphore for passenger signal
pthread_mutex_t lock;  // for updating rides
int iter;              // number of times request is made by passenger
int N_cars, N_pass;

int *avail_cars; // passenger id
int rides = 0;   // total rides

// car thread work
void *car_work(void *args)
{
    // deconstruction of paramters
    param *p = (param *)args;
    int index = p->id;
    int tr;

    // time variable
    timeval curTime;

    while (true)
    {
        sem_wait(&car_fill_seat[index]);  // check for vacent seat
        sem_post(&passenger);             // call passengers
        sem_wait(&car_seat_taken[index]); // wait till seat is filled

        // get passenger id
        int pass = avail_cars[index];
        if (pass == 0)
        {
            continue;
        }

        // print log
        fprintf(p->fp, "car %d accepted passenger's No:%d request\n", index + 1, pass);

        // update ride value
        pthread_mutex_lock(&lock);
        rides++;
        tr = rides;
        pthread_mutex_unlock(&lock);

        // print log
        fprintf(p->fp, "car %d completed passenger's No:%d request\n", index + 1, pass);

        // clear passenger id
        avail_cars[index] = 0;

        // sleep for time gap
        usleep(p->T * 1000);

        // terminate threads if rides completed
        if (tr == N_pass * iter)
        {
            char s[20];
            sprintf(s, "tour car %d exiting", index);
            pthread_exit(s);
        }
    }

    // free parameters
    free(args);
}

// passenger thread work
void *passgen_work(void *args)
{

    // deconstruction of paramters
    param *p = (param *)args;
    int index = p->id;
    timeval CT;
    int milli;
    char buffer[6];

    for (int i = 0; i < iter; i++)
    {

        // print log
        gettimeofday(&CT, NULL);
        milli = CT.tv_usec / 1000;
        strftime(buffer, 6, "%M:%S", localtime(&CT.tv_sec));
        fprintf(p->fp, "Passengers Thread No:% d in the museum at %s:%d \n", index + 1, buffer, milli);

        // print log
        gettimeofday(&CT, NULL);
        milli = CT.tv_usec / 1000;
        strftime(buffer, 6, "%M:%S", localtime(&CT.tv_sec));
        fprintf(p->fp, "Passenger %d made a ride request at %s:%d \n", index + 1, buffer, milli);

        // wait till any car is free
        sem_wait(&passenger);

        // select first free car in the order of index
        int avail_car;

        for (int i = 0; i < N_cars; i++)
        {
            if (avail_cars[i] == 0)
            {
                avail_car = i;
                avail_cars[i] = index + 1;
                sem_post(&car_seat_taken[i]);
                break;
            }
        }

        // print log
        gettimeofday(&CT, NULL);
        milli = CT.tv_usec / 1000;
        strftime(buffer, 6, "%M:%S", localtime(&CT.tv_sec));
        fprintf(p->fp, "Passenger %d started riding at %s:%d \n", index + 1, buffer, milli);

        // print log
        gettimeofday(&CT, NULL);
        milli = CT.tv_usec / 1000;
        strftime(buffer, 6, "%M:%S", localtime(&CT.tv_sec));
        fprintf(p->fp, "Passenger %d finished riding at %s:%d\n", index + 1, buffer, milli);

        // free the seat
        sem_post(&car_fill_seat[avail_car]);

        // sleep for time gap
        usleep(p->T * 1000);
    }

    // print if all request all completed
    fprintf(p->fp, "Passengers Thread No:% d has finished his tour .\n", index + 1);

    // free parameters
    free(args);
}

int main(int argc, char const *argv[])
{

    //take input from the file
    FILE *inp = fopen("inp-params.txt", "r");
    int lambda_P, lambda_C;
    fscanf(inp, "%d%d%d%d%d", &N_pass, &N_cars, &lambda_P, &lambda_C, &iter);
    fclose(inp);
 

    // open output file
    FILE *out = fopen("output.txt", "w");

    // exponential distribution

    int seed = chrono::system_clock::now().time_since_epoch().count();

    default_random_engine gen(seed);

    exponential_distribution<double> rng_1(lambda_P);
    exponential_distribution<double> rng_2(lambda_C);


   //intialize the thread ids and aslo the semaphores
    pthread_t *passengers = (pthread_t *)malloc(sizeof(pthread_t) * N_pass);
    pthread_t *cars = (pthread_t *)malloc(sizeof(pthread_t) * N_cars);
    car_seat_taken = (sem_t *)malloc(sizeof(sem_t) * N_cars);
    car_fill_seat = (sem_t *)malloc(sizeof(sem_t) * N_pass);
    ride_over = (sem_t *)malloc(sizeof(sem_t) * N_pass);
    avail_cars = (int *)calloc(sizeof(int), N_cars);
    sem_init(&passenger, 0, 0);


   //create car threads and pass parameters with random exp generation
    for (int i = 0; i < N_cars; i++)
    {

        param *p = (param *)malloc(sizeof(param));
        p->fp = out;
        p->T = rng_2(gen);
        p->id = i;
        sem_init(&car_fill_seat[i], 0, 1);
        sem_init(&car_seat_taken[i], 0, 0);

        pthread_create(&cars[i], NULL, car_work, (void *)p);
    }

   //create passengers threads and pass parameters with random exp generation 
    for (int i = 0; i < N_pass; i++)
    {
        param *p = (param *)malloc(sizeof(param));
        p->fp = out;
        p->T = rng_1(gen);
        p->id = i;
        sem_init(&ride_over[i], 0, 0);
        pthread_create(&passengers[i], NULL, passgen_work, (void *)p);
    }

  //wait till all passenger threads are terminated
    for (int i = 0; i < N_pass; i++)
    {
        pthread_join(passengers[i], NULL);
    }


   //free all the allocated memory
    free(car_fill_seat);
    free(car_seat_taken);
    free(passengers);
    free(cars);
    free(avail_cars);
    fclose(out);
    return 0;
}
