#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"
#include "queue.h"

#define NR_REQUESTS 1000

unsigned int seed;

void gen_cylinder_requests (int* req_arr, int nr_requests){
    int mod_roof = 5000;
    for (int i = 0; i < nr_requests; i++){
        req_arr[i] = rand() % mod_roof;
    }
}

int main (int argc, char* argv[]){
    int initial_pos = atoi(argv[1]);
    if (argc > 2) {
        seed = atoi(argv[2]);
        srand(seed);
    }

    int request_arr[NR_REQUESTS];
    gen_cylinder_requests(request_arr, NR_REQUESTS);


    int cyl_moves = FCFS(request_arr, NR_REQUESTS, initial_pos);
    printf ("FCFS cylinder movements: %d\n", cyl_moves);
    cyl_moves = SSTF(request_arr, NR_REQUESTS, initial_pos);
    printf ("SSTF cylinder movements: %d\n", cyl_moves);
    cyl_moves = SCAN (request_arr, NR_REQUESTS, initial_pos);
    printf ("SCAN cylinder movements: %d\n", cyl_moves);
    cyl_moves = LOOK (request_arr, NR_REQUESTS, initial_pos);
    printf ("LOOK cylinder movements: %d\n", cyl_moves);
    cyl_moves = C_SCAN (request_arr, NR_REQUESTS, initial_pos);
    printf ("C-SCAN cylinder movements: %d\n", cyl_moves);
    cyl_moves = C_LOOK (request_arr, NR_REQUESTS, initial_pos);
    printf ("C-LOOK cylinder movements: %d\n", cyl_moves);

    return 0;
}