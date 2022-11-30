#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

#define RIGHT_16BIT_MASK 0x0000ffff
#define PAGE_OFFSET_MASK 0x000000ff
#define READ_MODE "r"
#define READ_BUFFER_SIZE 32
#define MEM_SIZE (1<<16)

char* PHYSICAL_MEM;
char page_buffer[FRAME_SIZE + 1];

int debug_counter = 0;
char *read_buffer;



typedef struct page_info {
    int page_nr;
    int offset;
} page_info;

void print_debug(char* msg){
    printf("debug(Nr: %d, Msg: %s\n )\n", debug_counter++, msg);
}

void read_page_info (page_info *pi, int logaddr){
    logaddr &= RIGHT_16BIT_MASK;
    pi->offset = logaddr & PAGE_OFFSET_MASK; 
    logaddr >>= 8;
    pi->page_nr = logaddr & PAGE_OFFSET_MASK;
   // printf ("read page: %d \n", pi->page_nr);
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

int write_frame (char * page_buffer, int base_addr){
    int lastIndex = base_addr + FRAME_SIZE - 1;
    int i = base_addr;
    int j = 0;
    while (i <= lastIndex){
        PHYSICAL_MEM[i++] = page_buffer[j++];
    }
}

int main (int argc, char *argv[]){
    FILE *fptr;
    disc_reader disc; 

    page_info pifo;
    PHYSICAL_MEM = calloc(MEM_SIZE, sizeof(char));
    fifo *list = malloc(sizeof(fifo));

    fptr = fopen(argv[1], READ_MODE);
    if (fptr == NULL) {
        printf ("failed to open file, make sure path is correct!\n");
        return -1;
    }

    page_table pt;
    init_page_table(&pt);
    init_freelist(list);
    
    open_disk(&disc);

    read_buffer = malloc(sizeof(char) * READ_BUFFER_SIZE);
    size_t size = (size_t) READ_BUFFER_SIZE;
    int currAddr = -1;
    int baddr;
    int phys_addr;
    char stored_val; 
    while (getline(&read_buffer, &size, fptr) != -1){
        currAddr = atoi(read_buffer);
        read_page_info(&pifo, currAddr);
        while (is_page_free(&pt, pifo.page_nr)){
            baddr = get_free_frame_baddr(list);
            if (baddr == -1) {
                printf("Error: Out of physical memory!\n");
                return -1;
            }
            set_page_base_addr(&pt, pifo.page_nr, baddr);
            read_disk(&disc, page_buffer, pifo.page_nr);
            write_frame(page_buffer, baddr);
        }

        get_page_base_addr(&pt, pifo.page_nr, &phys_addr);
        phys_addr += pifo.offset;
        stored_val = PHYSICAL_MEM[phys_addr];
        printf("log addr: %d, phys addr: %d, value: %d\n", currAddr, phys_addr, stored_val);
    }
    
/*
    int taddr = 16916;
    read_page_info(&pifo, taddr);
    print_page_info(&pifo);
*/
    int base;
    get_page_base_addr(&pt, 2, &base);
    printf("val: %d\n", base);
    int fpgnr = find_base_addr(&pt, 256);
    printf ("pgnr found: %d\n", fpgnr);

    return 0;
}