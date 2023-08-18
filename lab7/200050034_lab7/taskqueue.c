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
bool done = false;
long current = 0;
long total = 0;
long* types;
long* nums;
pthread_mutex_t worker_lock;
pthread_mutex_t process_lock;

void processtask(long number);

void processtask(long number)
{
    sleep(number);
    
    pthread_mutex_lock(&process_lock);
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
    pthread_mutex_unlock(&process_lock);
}


void* deploy_worker() {
    while(true){
        pthread_mutex_lock(&worker_lock);
        if(current == total) {
        	pthread_mutex_unlock(&worker_lock);
            if(done) break;
            else continue;   
        }
        long task = current;
        current += 1;
        pthread_mutex_unlock(&worker_lock);
        if(types[task] == 0){
            processtask(nums[task]);
            printf("Task completed\n");
        }
        else {
            sleep(nums[task]);
            printf("Wait over\n");
        }
        //printf("Task %ld completed\n", task);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    FILE *fin = fopen(fn, "r");
    long t;
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    
    int workers = atoi(argv[2]);
    pthread_t th[workers];
    
    types = (long*) malloc(t * sizeof(long));
    nums = (long*) malloc(t * sizeof(long));
    
    for(int i = 0; i < workers; i++){
        if (pthread_create(th + i, NULL, &deploy_worker, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        //printf("Deployed worker %d\n", i);
    }
    
    char type;
    long num;
    long current;
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {   
        if (type == 'p')
        { 
            types[total] = 0;
            nums[total++] = num;
        }
        else if (type == 'w')
        { 
            types[total] = 1;
            nums[total++] = num;
        }
        else
        {
            printf("ERROR: Type Unrecognizable: '%c'\n", type);
            exit(EXIT_FAILURE);
        }
    }
    done = true;
    for (int i = 0; i < workers; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        //printf("Finished worker %d\n", i);
    }
    fclose(fin);
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    return (EXIT_SUCCESS);
}
