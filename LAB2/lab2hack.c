#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NR_THREADS 3
#define BUFFER_CEILING 15

typedef struct args {
    int thread_id;
    int thread_work;
    pthread_t thread_TID;
}args;

int buffer;
pthread_mutex_t lock;
volatile int blocked_id;

volatile int blocked_threads[2] ={-1,-1};

void update_blocked_threads (int id){
    blocked_threads[1] = blocked_threads[0];
    blocked_threads[0] = id;
}

void* thread_fun(void *arg){
    pthread_t tid = pthread_self();
    pid_t pid = getpid();

    args *argptr = (args*) arg;
    argptr->thread_TID = tid;
    //printf("start %d\n", argptr->thread_id);
    while (1){
        while (argptr->thread_id == blocked_threads[0] || argptr->thread_id == blocked_threads[1]);
        pthread_mutex_lock(&lock);
        if (buffer < BUFFER_CEILING){
            printf("TID: %ld, PID: %d, Buffer: %d\n", tid, pid, buffer);
            buffer++;
            argptr->thread_work++;
            update_blocked_threads(argptr->thread_id);
            pthread_mutex_unlock(&lock);
        }
        else{
            pthread_mutex_unlock(&lock);
            update_blocked_threads(argptr->thread_id);
            break;
        }
    }

    return NULL;
}

int main(void){
    buffer = 0;
    pthread_t t1, t2, t3;
    pthread_t *tarr_pointer[] = {&t1, &t2, &t3};
    args arguments[NR_THREADS];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NR_THREADS; i++){
        arguments[i].thread_id = i;
        arguments[i].thread_work = 0;
        pthread_create(tarr_pointer[i], NULL, &thread_fun, &arguments[i]);
    }

    for (int i = 0; i < NR_THREADS; i++){
        pthread_join(*tarr_pointer[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    
    for (int i = 0; i < NR_THREADS; i++){
        printf("TID: %ld worked on the buffer %d times\n", arguments[i].thread_TID, arguments[i].thread_work);
    }
    printf("Total buffer accesses: %d\n", buffer);

    return 0;
}