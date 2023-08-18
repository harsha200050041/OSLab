#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Global variables




// void create_threads(int n){
//     pthread_t th[n];
//     int i;
//     for(i=0;i<n;i++){
//         pthread_create(th[i],NULL,&processtask,(void*) arg);
//     }
// }


long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;
int current_working = 0;
int total;
long* types;
long* numbers;
pthread_mutex_t processtask_lock;
pthread_mutex_t worker_lock;

// void set_total(void* n){
//     total = (int *)n;

// }
void processtask(long number);

void processtask(long number)
{
    // simulate burst time
    sleep(number);

    // update global variables
    sum += number;
    pthread_mutex_lock(&processtask_lock);
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
    pthread_mutex_unlock(&processtask_lock);
}

void* worker(void* arg)
{
    int a = (int *) arg;
    while(true){
        pthread_mutex_lock(&worker_lock);
        if(current_working==a){
            pthread_mutex_unlock(&worker_lock);
            if(done){
                break;
            }
            else{
                continue;
            }
        }
        long present = current_working;
        current_working++;
        pthread_mutex_unlock(&worker_lock);    
            printf("wow\n");
            if(types[present]==1){
                processtask(numbers[present]);
            }
            else{
                sleep(numbers[present]);
            }
            
    
            
        
        
    }
}


int main(int argc, char *argv[])
{
    int n;
    //input number of threads
    scanf("%d", &n);
    pthread_t main;
    total = n;
    // pthread_create(&main,NULL,&set_total,(void*) n);
    //create threads
    

    if (argc != 2)
    {
        printf("Usage: sum <infile>\n");
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
    int task =0;
    pthread_t th[n];
    for(int i=0;i<n;i++){
        printf("entered\n");
        if(pthread_create(&th[i],NULL,&worker,(void*) n) != 0){
            perror("Failed to create thread");
            return 1;
        }
    }
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {
        printf("ehee\n");
        if (type == 'p')
        { // processing task

            // processtask(num);
            
            types[task]=1;
            numbers[task++]=num;
            printf("Task completed\n");
        }
        else if (type == 'w')
        { // waiting period
            types[task]=2;
            numbers[task++]=num;
            printf("Wait Over\n");
        }
        else
        {
            printf("ERROR: Type Unrecognizable: '%c'\n", type);
            exit(EXIT_FAILURE);
        }
    }
    done = true;
    for (int i = 0; i < n; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        //printf("Finished worker %d\n", i);
    }
    // pthread_join(main,NULL);
    fclose(fin);
    // Print global variables
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    return (EXIT_SUCCESS);
}

// #include <limits.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <pthread.h>

// // Global variables
// long sum = 0;
// long odd = 0;
// long even = 0;
// long min = INT_MAX;
// long max = INT_MIN;
// bool done = false;
// long current = 0;
// long total = 0;
// long* types;
// long* nums;
// pthread_mutex_t worker_lock;
// pthread_mutex_t process_lock;

// void processtask(long number);

// void processtask(long number)
// {
//     sleep(number);
    
//     pthread_mutex_lock(&process_lock);
//     sum += number;
//     if (number % 2 == 1)
//     {
//         odd++;
//     }
//     else
//     {
//         even++;
//     }
//     if (number < min)
//     {
//         min = number;
//     }
//     if (number > max)
//     {
//         max = number;
//     }
//     pthread_mutex_unlock(&process_lock);
// }


// void* deploy_worker() {
//     while(true){
//         pthread_mutex_lock(&worker_lock);
//         if(current == total) {
//         	pthread_mutex_unlock(&worker_lock);
//             if(done) break;
//             else continue;   
//         }
//         long task = current;
//         current += 1;
//         pthread_mutex_unlock(&worker_lock);
//         if(types[task] == 0){
//             processtask(nums[task]);
//             printf("Task completed\n");
//         }
//         else {
//             sleep(nums[task]);
//             printf("Wait over\n");
//         }
//         //printf("Task %ld completed\n", task);
//     }
// }

// int main(int argc, char *argv[])
// {
//     if (argc != 3)
//     {
//         exit(EXIT_FAILURE);
//     }
//     char *fn = argv[1];
//     FILE *fin = fopen(fn, "r");
//     long t;
//     fscanf(fin, "%ld\n", &t);
//     printf("The number of tasks are : %ld \n", t);
    
//     int workers = atoi(argv[2]);
//     pthread_t th[workers];
    
//     types = (long*) malloc(t * sizeof(long));
//     nums = (long*) malloc(t * sizeof(long));
    
//     for(int i = 0; i < workers; i++){
//         if (pthread_create(th + i, NULL, &deploy_worker, NULL) != 0) {
//             perror("Failed to create thread");
//             return 1;
//         }
//         //printf("Deployed worker %d\n", i);
//     }
    
//     char type;
//     long num;
//     long current;
//     while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
//     {   
//         if (type == 'p')
//         { 
//             types[total] = 0;
//             nums[total++] = num;
//         }
//         else if (type == 'w')
//         { 
//             types[total] = 1;
//             nums[total++] = num;
//         }
//         else
//         {
//             printf("ERROR: Type Unrecognizable: '%c'\n", type);
//             exit(EXIT_FAILURE);
//         }
//     }
//     done = true;
//     for (int i = 0; i < workers; i++) {
//         if (pthread_join(th[i], NULL) != 0) {
//             return 2;
//         }
//         printf("Finished worker %d\n", i);
//     }
//     fclose(fin);
//     printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

//     return (EXIT_SUCCESS);
// }