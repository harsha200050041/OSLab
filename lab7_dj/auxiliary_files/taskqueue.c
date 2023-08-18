#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


// Global variables
long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;
long* types;
long* nums;
int curr_task = 0;
int total_threads;
int total_tasks = 0;
int counter=0;
bool done = false;
pthread_mutex_t worker = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t task = PTHREAD_MUTEX_INITIALIZER;

void processtask(long number);

void processtask(long number)
{
    // simulate burst time
    sleep(number);
    pthread_mutex_lock(&task);
    // update global variables
    sum += number;
    if (number % 2 == 1)
    {
        odd++;
    }
    else
    {
        even++;
    }
    if (number < min)
    {
        min = number;
    }
    if (number > max)
    {
        max = number;
    }
    pthread_mutex_unlock(&task);
    
}

void* deploy_worker(void* arg){
    
    int* n = (int *) arg;
    while(true){
        if(done){
            break;
        }
        pthread_mutex_lock(&worker);
        long counter1 = counter;
        counter++;
        if(counter1==*n){
            pthread_mutex_unlock(&worker);
            continue;
        }
        pthread_mutex_unlock(&worker);
        else{
            pthread_mutex_unlock(&worker);
            if(types[counter1]==1){
                processtask(nums[counter1]);
                
            }
            else{
                sleep(nums[counter1]);
                
            }
        }
        
    }
}

int main(int argc, char *argv[])
{

    
    if (argc != 3)
    {
        printf("Usage: sum <infile> <numberofthreads>\n");
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    // Read from file
    FILE *fin = fopen(fn, "r");
    long t;
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    char type;
    long num;
    printf("The tasks are : \n");

    int threads = atoi(argv[2]);

    printf("The number of threads are : %d \n", threads);
    pthread_t the[threads];

    for(int i=0;i<threads;i++){
        if(pthread_create(&the[i],NULL,deploy_worker,(void *)&threads)){
            printf("Error creating thread");
        }
    }

    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {
        printf("gone");
        if (type == 'p')
        { // processing task
            types[total_tasks] = 1;
            nums[total_tasks] = num;
            total_tasks++;
            printf("Task completed\n");
        }
        else if (type == 'w')
        { // waiting period

            types[total_tasks] = 2;
            nums[total_tasks] = num;
            total_tasks++;

            // sleep(num);
            printf("Wait Over\n");
        }
        else
        {
            printf("ERROR: Type Unrecognizable: '%c'\n", type);
            exit(EXIT_FAILURE);
        }
    }
    // fclose(fin);
    
    // // Print global variables
    // for(int i=0;i<threads;i++){
    //     if(pthread_join(the[i],NULL)){
    //         printf("Error joining thread");
    //     }
    // }
    // done = true;
    // printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    // return (EXIT_SUCCESS);
}
