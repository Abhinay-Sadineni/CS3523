#include <iostream>
#include <cstdlib>
#include <fstream>
#include <pthread.h>
#include <time.h>
#include <chrono>


using namespace std;
using namespace std::chrono;

struct points{
 float x;
 float y;
 bool circle;
};


struct points*Generated_set;//global pointer for random point
int N,K; //global variables: number of points,number of threads


//Each thread work
//Generates points and determines whether in or out of circle
//stores them in global array
//Made sure to distribute number of points evenly among threads 
void *thread_work(void*par){
     
     int i=*(int*)par;
     int d=N/K;
     int r=N%K;
     int start=i*d;

    //this block of code gives start index and size of the data 
     if(r!=0){
     if(i<r){
        d=d+1;
        start=i*d;
     }
     else{
        start=(r)*(d+1)+(i-r)*(d);
     }
     }


    //generate the point and determine whether it is in or out of circle
     float z,w;

     for (int j = start; j < start+d; j++)
     {
        Generated_set[j].x=z=2*(float)(rand())/(RAND_MAX) -1;
        Generated_set[j].y=w=2*(float)(rand())/(RAND_MAX) -1;
        Generated_set[j].circle=false;
        if(z*z+w*w <=1){
            Generated_set[j].circle=true;
        }
     }
     delete (int*)par;
     pthread_exit(0);
}



//main thread work
float main_thread_work(){
      int no_of_pt_circle=0;

      //This function will count the no of points in circle
      //and compute the pi value
      for (int i = 0; i < N; i++)
      {
        if(Generated_set[i].circle==true){
              no_of_pt_circle+=1;
        }
      
      }
      return 4*float(no_of_pt_circle)/N;
}



void log_creation(){
    ofstream output;
    output.open("output.txt",ios::in|ios::out);
    output.seekp (0, ios::end);
        int i,j;
    
    output<<"Log:"<<endl<<endl;    

    for ( i = 0; i < K; i++)
    {

     //determining the starting index and size of data splited for each thread
     int d=N/K;
     int r=N%K;
     int start=i*d;
     int no_circle=0;

     if(r!=0){
     if(i<r){
        d=d+1;
        start=i*d;
     }
     else{
        start=(r)*(d+1)+(i-r)*(d);
     }
     }

    //printing out log 
    //with use of global array
        output<<"Thread"<<i<<":"<<" ";
        long pos_1=output.tellp();
        output<<endl<<endl<<endl;

        output<<"         Inside circle: ";

        for (j = start; j <start+d; j++)
        {
            if(Generated_set[j].circle==true){
            no_circle++;
            output<<"("<<Generated_set[j].x<<","<<Generated_set[j].y<<") ";
            }
        }
        output<<endl;
        output<<"Inside square: ";
        for ( j = i*N/K; j < (i+1)*(N/K); j++)
        {
            output<<"("<<Generated_set[j].x<<","<<Generated_set[j].y<<") ";
        }
        output<<endl<<endl<<endl;
        long pos_2=output.tellp();
        output.seekp(pos_1);
        output<<no_circle<<","<<d<<endl;
        output.seekp(pos_2);
        no_circle=0;
    }


    output.close();
}


int main(int argc,char**argv){
      
     
    ifstream myfile; //file pointer 
    myfile.open("inp.txt",fstream::in);
    myfile>>N>>K; //scan input parameter
    myfile.close();
 

    
    auto start = high_resolution_clock::now();//get current time


    Generated_set=(points*)malloc(sizeof(points)*N); //allocation of memory for random points
    pthread_t *tid=(pthread_t*)malloc(sizeof(pthread_t)*K); //allocation of memory for thread identifiers
    


    srand(time(NULL));//set seed for random

    for (int i = 0; i < K; i++)
    {
     pthread_create(&tid[i],NULL,thread_work,new int(i)); //create threads and assign task
    }
       
    for (size_t i = 0; i < K; i++)
    {
        pthread_join(tid[i],NULL);//
    }
       
    float computed_value=main_thread_work(); //Computued value of pi (main thread)
    auto end = high_resolution_clock::now(); //get current time


    auto duration = duration_cast<microseconds>(end - start);//get the execution time
 
    
    //writing to output file
    ofstream output;
    output.open("output.txt",fstream::out);
    output<<"Execution time:"<<duration.count()<<"\xC2\xB5s"<<endl<<endl;
    output<<"Computed_value:"<<computed_value<<endl<<endl;
    output.close();
    cout<<"Generating log file, please be paitent"<<endl;


    //creating log 
    log_creation();
    cout<<"completed please see output.txt"<<endl;

    //free the allocated memory
    free(Generated_set);
    free(tid); 
    return 0;
}
