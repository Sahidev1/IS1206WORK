#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NR_THREADS 3
#define BUFFER_CEILING 15

int buffer;
pthread_mutex_t lock;

void* thread_fun(void *arg){
    pthread_t tid = pthread_self();
    pid_t pid = getpid();

    int *idptr = (int*) arg;
    //printf("start %d\n", *idptr);
    while (1){
        pthread_mutex_lock(&lock);
        if (buffer < BUFFER_CEILING){
            printf("TID: %ld, PID: %d, Buffer: %d\n", tid, pid, buffer);
            buffer++;
            pthread_mutex_unlock(&lock);
        }
        else{
            pthread_mutex_unlock(&lock);
            break;
        }
    }

    return NULL;
}



int main(void){
    buffer = 0;
    pthread_t t1, t2, t3;
    pthread_t *tarr_pointer[] = {&t1, &t2, &t3};
    int index[] = {1,2,3};

    pthread_mutex_init(&lock, NULL);
    /*
    pthread_create(&t1, NULL, &thread_fun, NULL);
    */
    for (int i = 0; i < NR_THREADS; i++){
        pthread_create(tarr_pointer[i], NULL, &thread_fun, index + i);
    }

    for (int i = 0; i < NR_THREADS; i++){
        pthread_join(*tarr_pointer[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    return 0;
}