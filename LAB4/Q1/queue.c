#include "queue.h"
#include "stdlib.h"
#include "stdio.h"

void init_queue (queue *q){
    q->curr_entries = 0;
    q->head = NULL;
    q->tail = NULL;
}

void put_request (queue *q, int cylinder_request){
    node* new_node = malloc(sizeof(node));
    new_node->cylinder_request = cylinder_request;
    node* tail = q->tail;
    new_node->next = NULL;
    new_node->prev = tail;
    q->curr_entries++;
    if (tail == NULL){
        q->tail = new_node;
        q->head = new_node;
        return;
    }
    tail->next = new_node;
    q->tail = new_node;
}

int pull_first_request (queue *q){
    node *headptr = q->head;
    if (headptr == NULL) return -1;
    int cylinder_pos = headptr->cylinder_request;
    if (headptr->next != NULL){
        q->head = headptr->next;
        q->head->prev = NULL;
    }
    else {
        q->head = NULL;
        q->tail = NULL;
    }
    q->curr_entries--;
    free (headptr);
    return cylinder_pos;
}

int pop_request (queue *q){
    node *tailptr = q->tail;
    if (tailptr == NULL) return -1;
    int cylinder_pos = tailptr->cylinder_request;
    if (tailptr->prev != NULL){
        q->tail = tailptr->prev;
        q->tail->next = NULL;
    }
    else {
        q->head = NULL;
        q->tail = NULL;
    }
    q->curr_entries--;
    free (tailptr);
    return cylinder_pos;
}

int pull_specific_request (queue *q, node *request_node){
    if (request_node == NULL) return -1;
    if (request_node == q->head) return pull_first_request (q);
    if (request_node == q->tail) return pop_request(q);
    int request_val = request_node->cylinder_request;
    node* prev_to = request_node->prev;
    node* next_to = request_node->next;
    prev_to->next = next_to;
    next_to->prev = prev_to;
    q->curr_entries--;
    free(request_node);
    return request_val;
} 

int pull_closest_request (queue *q, int curr_head){
    node *head = q->head;
    if (head == NULL) return -1;

    node *closest_ptr = head;
    int closest_val;
    while (head != NULL){
        if (abs(curr_head - head->cylinder_request) < abs(curr_head - closest_ptr->cylinder_request)){
            closest_ptr = head;
        }
        head = head->next;
    }
    return pull_specific_request (q, closest_ptr);
}

int search_cylinder (queue *request_queue, int cylinder_number){
    node *head = request_queue->head;
    int hits = 0;
    node *tmp;
    while (head != NULL){
        if (head->cylinder_request == cylinder_number){
            hits++;
            tmp = head;
            head = head->next;
            pull_specific_request(request_queue, tmp);
        }
        else {
            head = head->next;
        }
    }
    return hits;
}