#include <stdio.h>

#ifndef STRUCTURES_H_
#define STRUCTURES_H

#define PAGE_TABLE_SIZE 256
#define FRAME_SIZE 256
#define SET_USED 0x80000000
#define DISK_PATH "data/BACKING_STORE.bin"
#define RIGHT_16BIT_MASK 0x0000ffff
#define PAGE_OFFSET_MASK 0x000000ff
#define READ_MODE "r"

typedef struct page_info {
    int page_nr;
    int offset;
} page_info;

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

void read_page_info (page_info *pi, int logaddr);

void print_page_info (page_info *pi);

void open_disk (disc_reader *disc);

int read_disk (disc_reader *disc, char* buffer ,int pgnr);

int write_frame (char *page_buffer, char *PHYS_MEM, int base_addr);

#endif