
#ifndef QUEUE_H_
#define QUEUE_H

typedef struct node {
    int cylinder_request;
    struct node *next;
    struct node *prev;
}node;

typedef struct queue {
    int curr_entries;
    node *head;
    node *tail;
}queue;

void init_queue (queue *q);
void put_request (queue *q, int cylinder_request);
int pull_first_request (queue *q);
int pull_closest_request (queue *q, int curr_head);
int search_cylinder (queue *request_queue, int cylinder_number);

#endif