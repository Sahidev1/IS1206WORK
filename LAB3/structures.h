

#ifndef STRUCTURES_H_
#define STRUCTURES_H

#define PAGE_TABLE_SIZE 256
#define FRAME_SIZE 256
#define SET_USED 0x80000000

typedef struct fnode {
    int frame_address;
    struct fnode *next;
    struct fnode *prev;
}fnode;

typedef struct doubly_linked_list{
    fnode *curr;
    fnode *head;
    fnode *tail;
}doubly;

typedef struct page_table {
    int page_array[PAGE_TABLE_SIZE];
}page_table;

int get_page_base_addr (page_table *pt, int pgnr, int *base_addr_ptr);

int set_page_base_addr (page_table *pt, int pgnr, int base_addr);

int find_base_addr (page_table *pt, int base_addr);

void init_page_table(page_table *pt);


int init_freelist (doubly *freelist);

int pop_curr_frame (doubly *freelist);

int init_list (doubly *freelist);

#endif