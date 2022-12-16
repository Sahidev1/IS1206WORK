#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void fill_queue (queue *q, int* requests, int req_size){
    for (int i = 0; i < req_size; i++){
        put_request(q, requests[i]);
    }
}

int FCFS (int *req_arr, int arr_size, int initial_headpos){
    int cylinder_movements = 0;
    int headpos = initial_headpos;
    queue *q = malloc(sizeof(queue));
    init_queue(q);
    fill_queue (q, req_arr, arr_size);

    int cylinder_req;
    while (q->curr_entries > 0){
        cylinder_req = pull_first_request(q);
        cylinder_movements += abs(headpos - cylinder_req);
        headpos = cylinder_req;
    }
    free (q);
    return cylinder_movements;
}

int SSTF (int *req_arr, int arr_size, int initial_headpos){
    printf("henlo\n");
    int cylinder_movements = 0;
    int headpos = initial_headpos;
    queue *q = malloc (sizeof(queue));
    init_queue(q);
    fill_queue(q, req_arr, arr_size);

    int cylinder_req;
    while (q->curr_entries > 0){
        cylinder_req = pull_closest_request(q, headpos);
        cylinder_movements += abs(headpos - cylinder_req);
        headpos = cylinder_req;
    }
    free(q);
    return cylinder_movements;
}