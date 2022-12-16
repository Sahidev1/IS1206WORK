#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"
#include "queue.h"

#define NR_REQUESTS 1000

void gen_cylinder_requests (int* req_arr, int nr_requests){
    int mod_roof = 5000;
    for (int i = 0; i < nr_requests; i++){
        req_arr[i] = rand() % mod_roof;
    }
}

int main (int argc, char* argv[]){
    int initial_pos = atoi(argv[1]);
    int request_arr[NR_REQUESTS];
    gen_cylinder_requests(request_arr, NR_REQUESTS);


    int cyl_moves = FCFS(request_arr, NR_REQUESTS, initial_pos);
    printf ("cylinder movements: %d\n", cyl_moves);
    cyl_moves = SSTF(request_arr, NR_REQUESTS, initial_pos);
    printf ("cylinder movements: %d\n", cyl_moves);

    return 0;
}