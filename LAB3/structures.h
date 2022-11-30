#include <stdio.h>

#ifndef STRUCTURES_H_
#define STRUCTURES_H

#define PAGE_TABLE_SIZE 256
#define FRAME_SIZE 256
#define SET_USED 0x80000000
#define DISK_PATH "data/BACKING_STORE.bin"

typedef struct fnode {
    int frame_address;
    struct fnode *next;
}fnode;

typedef struct fifo{
    fnode *head;
}fifo;

typedef struct disc_reader{
    FILE *fstream;
}disc_reader;

typedef struct page_table {
    int page_array[PAGE_TABLE_SIZE];
}page_table;

int get_page_base_addr (page_table *pt, int pgnr, int *base_addr_ptr);

int set_page_base_addr (page_table *pt, int pgnr, int base_addr);

int find_base_addr (page_table *pt, int base_addr);

void init_page_table(page_table *pt);

int is_page_free(page_table *pt, int pgnr);

int init_freelist (fifo *freelist);

int get_free_frame_baddr (fifo *freelist);

int deque_free_frame (fifo *freelist, int *addr);

#endif