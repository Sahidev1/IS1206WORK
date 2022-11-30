#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

void init_page_table(page_table *pt){
    for(int i = 0; i < PAGE_TABLE_SIZE; i++){
        pt->page_array[i] = 0;
    }
    //pt->page_array[PAGE_TABLE_SIZE - 1] -= 1;
}

int get_page_base_addr (page_table *pt, int pgnr, int *base_addr_ptr){
    if (pgnr >= PAGE_TABLE_SIZE) return -1;
    *base_addr_ptr = pt->page_array[pgnr]&(~(SET_USED));
    return 0;
}

int set_page_base_addr (page_table *pt, int pgnr, int base_addr){
    if (pgnr >= PAGE_TABLE_SIZE) return -1;
    if (pt->page_array[pgnr] == (base_addr | SET_USED)) return -2;
    pt->page_array[pgnr] = base_addr | SET_USED;
    return 0;
}

int find_base_addr (page_table *pt, int base_addr){
    for (int i = 0; i < PAGE_TABLE_SIZE; i++){
        if (pt->page_array[i] == (base_addr|SET_USED)){
            return i;
        }
    }
    return -1;
}

int init_list (doubly *freelist){
    init_freelist(freelist);
    return 0;
}

int init_freelist (doubly *freelist){
    freelist->head = malloc (sizeof(fnode));
    fnode *head = freelist->head;
    head->prev = NULL;
    freelist->curr = head;
    fnode *iterator = head;
    fnode *temp;
    int i = 0;
    while(i < FRAME_SIZE){
        iterator->frame_address = 256*i;
        iterator->next = malloc(sizeof(fnode));
        temp = iterator;
        iterator = iterator->next;
        iterator->prev = temp;
        i++;
    }
    freelist->tail = iterator;
    return 0;
}

int get_free_frame_baddr (doubly *freelist){
    
}

int pop_curr_frame (doubly *freelist){
        printf("boop");
    fnode *freelist_node = freelist->curr;
    fnode *prev = freelist_node->prev;
    fnode *next = freelist_node->next;
    if (prev != NULL){
        prev->next = next;
    }
    else {
        if (next != NULL){
            next->prev = NULL;
            freelist->head = next;
        }
    }
    if (next != NULL){
        next->prev = prev;
    }
    else {
        if (prev != NULL){
            prev->next = NULL;
            freelist->tail = prev;
        }
    }
    free (freelist_node);
    return 0;
}