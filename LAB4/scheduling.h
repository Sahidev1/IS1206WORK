
#ifndef SCHEDULING_H_
#define SCHEDULING_H

int FCFS (int* cylinder_request, int request_arr_size, int initial_headpos);

int SSTF (int *req_arr, int arr_size, int initial_headpos);

int SCAN (int *req_arr, int arr_size, int initial_headpos);

int LOOK (int *req_arr, int arr_size, int initial_headpos);

#endif