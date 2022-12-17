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

int scanner (queue *q, int start_pos, int start_cyl, int end_cyl){
    int pos = start_pos;
    int last_hitpos = start_pos;
    int cylinder_movements = 0;
    int hits;
    while (pos >= start_cyl){
        hits = search_cylinder(q, pos);
        if (hits > 0){
            //printf("%d->", pos);
            cylinder_movements += abs(pos - last_hitpos);
            last_hitpos = pos;
        }
        if (pos == 0){
            //triggered by SCAN algorithm
            // head comes to last cylinder and reverses
            last_hitpos = 0;
        }
        pos--;
    }
    while (pos <= end_cyl){
        hits = search_cylinder(q, pos);
        if (hits > 0){
            //printf("%d->", pos);
            cylinder_movements += abs(pos - last_hitpos);
            last_hitpos = pos;
        }
        pos++;
    }
    return cylinder_movements;
}

int SCAN (int *req_arr, int arr_size, int initial_headpos){
    queue *q = malloc (sizeof(queue));
    init_queue(q);
    fill_queue(q, req_arr, arr_size);
    int cylinder_movements = scanner(q, initial_headpos, 0, 4999);
    free(q);
    return cylinder_movements;
}


int LOOK (int *req_arr, int arr_size, int initial_headpos){
    int lowest_cyl = 4999;
    int highest_cyl = 0;
    for (int i = 0; i < arr_size; i++){
        if (req_arr[i] < lowest_cyl){
            lowest_cyl = req_arr[i];
        }
        if (req_arr[i] > highest_cyl){
            highest_cyl = req_arr[i];
        }
    }
    printf ("lowest cyl: %d, highest cyl: %d \n", lowest_cyl, highest_cyl);
    queue *q = malloc (sizeof(queue));
    init_queue(q);
    fill_queue(q, req_arr, arr_size);
    int cylinder_movements = scanner(q, initial_headpos, lowest_cyl, highest_cyl);
    free(q);
    return cylinder_movements;
}