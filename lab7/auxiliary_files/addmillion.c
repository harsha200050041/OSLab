#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int account_balance = 0;

void* increment(void* arg) {
    int *threadnum = (int *)arg;
    for (int i = 0; i < 1000000*2048/(*threadnum); i++) {
        account_balance++;
    }
}

int main(int argc, char* argv[]) {
    clock_t start;
    start = clock();
    int threadNum ;
    scanf("%d",&threadNum);
    pthread_t th[threadNum];
    int i;
    
    for (i = 0; i < threadNum; i++) {
        if (pthread_create(th + i, NULL, &increment, (void*) &threadNum) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        pthread_join(th[i],NULL);
        printf("Transaction %d has started\n", i);
    }
    
    printf("Account Balance is : %d\n", account_balance);
    // for (i = 0; i < threadNum; i++) {
    //     if (pthread_join(th[i], NULL) != 0) {
    //         return 2;
    //     }
    //     printf("Transaction %d has finished\n", i);
    // }
    printf("Time spent:%f ms\n",(double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}