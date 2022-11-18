/*
    Author: Ali Sahibi
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NR_THREADS 3
#define BUFFER_CEILING 15
#define SLEEP_FOR_MICROS 10

volatile int buffer;
pthread_mutex_t lock;

typedef struct thread_retval {
    pthread_t tid;
    int thread_work;
}thread_retval;

void sleep_micros(int micro_seconds){
    struct timespec t;
    int nanos_per_micro = 1000;
    t.tv_nsec = micro_seconds * nanos_per_micro;
    t.tv_sec = 0;
    nanosleep (&t, NULL);
}

void* thread_fun(void *arg){
    pthread_t tid = pthread_self();
    pid_t pid = getpid();

    thread_retval *ret = malloc(sizeof(thread_retval));
    ret->thread_work = 0;
    ret->tid = tid; 

    while (1){
        pthread_mutex_lock(&lock);
        if (buffer < BUFFER_CEILING){
            printf("TID: %ld, PID: %d, Buffer: %d\n", tid, pid, buffer);
            buffer++;
            pthread_mutex_unlock(&lock);
            ret->thread_work++;
        }
        else{
            pthread_mutex_unlock(&lock);
            break;
        }
        sleep_micros(SLEEP_FOR_MICROS);
    }
    pthread_exit(ret);
}

int main(void){
    buffer = 0;
    pthread_t t[NR_THREADS];
    void *retptrs[NR_THREADS];
    thread_retval *retval;

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NR_THREADS; i++){
        pthread_create(&t[i], NULL, &thread_fun, NULL);
    }

    for (int i = 0; i < NR_THREADS; i++){
        pthread_join(t[i], &retptrs[i]);
    }
    
    pthread_mutex_destroy(&lock);

    for (int i = 0; i < NR_THREADS; i++){
        retval =  (thread_retval*) retptrs[i];
        printf("TID: %ld worked on the buffer %d times\n", retval->tid, retval->thread_work);
        free(retval);
    }
    return 0;
}