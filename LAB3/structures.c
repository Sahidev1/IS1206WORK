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

/* checks if page slot is free, page slots that have never
    been written to have free bit set to 0, free bit is
    MSB.
*/
int is_page_free(page_table *pt, int pgnr){
    int addr = pt->page_array[pgnr];
    return (addr & SET_USED) == 0x0;
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

int init_freelist (fifo *freelist){
    freelist->head = malloc (sizeof(fnode));
    fnode *iterator = freelist->head;
    int i = 0;
    while(i < FRAME_SIZE){
        iterator->frame_address = 256*i;
        iterator->next = malloc(sizeof(fnode));
        iterator = iterator->next;
        i++;
    }
    return 0;
}

int get_free_frame_baddr (fifo *freelist){
    int addr = -1;
    deque_free_frame(freelist, &addr);
    return addr;
}

int deque_free_frame (fifo *freelist, int *addr){
    if (freelist->head == NULL) return -1;
    fnode *head = freelist->head;
    *addr = head->frame_address;
    freelist->head = freelist->head->next;
    free (head);
    return 0;
}

void read_page_info (page_info *pi, int logaddr){
    logaddr &= RIGHT_16BIT_MASK;
    pi->offset = logaddr & PAGE_OFFSET_MASK; 
    logaddr >>= 8;
    pi->page_nr = logaddr & PAGE_OFFSET_MASK;
}

void print_page_info (page_info *pi){
    printf("Page Nr: %d, Offset: %d \n", pi->page_nr, pi->offset);
}

void open_disk (disc_reader *disc){
    disc->fstream = fopen(DISK_PATH, READ_MODE);
}

int read_disk (disc_reader *disc, char* buffer ,int pgnr){
    int offset = pgnr*256;
    fseek(disc->fstream, offset, SEEK_SET);
    fread(buffer, FRAME_SIZE + 1, 1, disc->fstream);
    rewind(disc->fstream);
    return 0;
}

int write_frame (char *page_buffer, char *PHYS_MEM, int base_addr){
    int lastIndex = base_addr + FRAME_SIZE - 1;
    int i = base_addr;
    int j = 0;
    while (i <= lastIndex){
        PHYS_MEM[i++] = page_buffer[j++];
    }
}