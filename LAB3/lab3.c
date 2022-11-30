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
    pi->page_nr = logaddr & PAGE_OFFSET_MASK; 
    logaddr >>= 8;
    pi->offset = logaddr & PAGE_OFFSET_MASK;
}

void print_page_info (page_info *pi){
    printf("Page Nr: %d, Offset: %d \n", pi->page_nr, pi->offset);
}

int main (int argc, char *argv[]){
    FILE *fptr;
    page_info pifo;
    PHYSICAL_MEM = calloc(MEM_SIZE, sizeof(char));

    fptr = fopen(argv[1], READ_MODE);
    if (fptr == NULL) {
        printf ("failed to open file, make sure path is correct!\n");
        return -1;
    }

    page_table pt;
    init_page_table(&pt);

    read_buffer = malloc(sizeof(char) * READ_BUFFER_SIZE);
    size_t size = (size_t) READ_BUFFER_SIZE;
    int currAddr = -1;
    while (getline(&read_buffer, &size, fptr) != -1){
        currAddr = atoi(read_buffer);
        read_page_info(&pifo, currAddr);
        //print_page_info(&pifo);
    }

    int base;
    get_page_base_addr(&pt, 2, &base);
    printf("val: %d\n", base);
    int fpgnr = find_base_addr(&pt, 256);
    printf ("pgnr found: %d\n", fpgnr);

    return 0;
}