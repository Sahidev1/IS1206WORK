#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

#define READ_BUFFER_SIZE 32
#define MEM_SIZE (1<<16)

char* PHYSICAL_MEMORY;
char page_buffer[FRAME_SIZE + 1];

int debug_counter = 0;
char *read_buffer;

void print_debug(char* msg){
    printf("debug(Nr: %d, Msg: %s\n )\n", debug_counter++, msg);
}

int main (int argc, char *argv[]){
    FILE *fptr;
    disc_reader disc; 
    page_info pifo;
    fifo *list;
    page_table pt;

    PHYSICAL_MEMORY = calloc(MEM_SIZE, sizeof(char));
    list = malloc(sizeof(fifo));

    fptr = fopen(argv[1], READ_MODE);
    if (fptr == NULL) {
        printf ("failed to open file, make sure path is correct!\n");
        return -1;
    }

    init_page_table(&pt);
    init_freelist(list);
    open_disk(&disc);

    read_buffer = malloc(sizeof(char) * READ_BUFFER_SIZE);
    size_t size = (size_t) READ_BUFFER_SIZE;
    int virtual_address = -1;
    int base_address;
    int physical_address;
    char stored_value; 
    while (getline(&read_buffer, &size, fptr) != -1){
        virtual_address = atoi(read_buffer);
        read_page_info(&pifo, virtual_address);
        while (is_page_free(&pt, pifo.page_nr)){
            base_address = get_free_frame_baddr(list);
            if (base_address == -1) {
                printf("Error: Out of physical memory!\n");
                return -1;
            }
            set_page_base_addr(&pt, pifo.page_nr, base_address);
            read_disk(&disc, page_buffer, pifo.page_nr);
            write_frame(page_buffer, PHYSICAL_MEMORY ,base_address);
        }

        get_page_base_addr(&pt, pifo.page_nr, &physical_address);
        physical_address += pifo.offset;
        stored_value = PHYSICAL_MEMORY[physical_address];
        printf("log addr: %d, phys addr: %d, value: %d\n", virtual_address, physical_address, stored_value);
    }
    return 0;
}