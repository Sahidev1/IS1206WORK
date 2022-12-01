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

int TBL_hit_count, page_fault_count;

void print_debug(char* msg){
    printf("debug(Nr: %d, Msg: %s\n )\n", debug_counter++, msg);
}

void print_data (int virtual_address, int physical_address, int stored_value){
        printf("Virtual address: %d, Physical address: %d, Value stored: %d\n", virtual_address, physical_address, stored_value);
}

int main (int argc, char *argv[]){
    TBL_hit_count = 0;
    page_fault_count = 0;
    FILE *fptr;
    disc_reader disc; 
    page_info pifo;
    fifo *list;
    page_table pt;
    TBL *tbl_cache;


    PHYSICAL_MEMORY = calloc(MEM_SIZE, sizeof(char));
    list = malloc(sizeof(fifo));
    tbl_cache = malloc(sizeof(TBL));

    fptr = fopen(argv[1], READ_MODE);
    if (fptr == NULL) {
        printf ("failed to open file, make sure path is correct!\n");
        return -1;
    }


    init_page_table(&pt);
    init_freelist(list);
    open_disk(&disc);
    init_TBL(tbl_cache);

    read_buffer = malloc(sizeof(char) * READ_BUFFER_SIZE);
    size_t size = (size_t) READ_BUFFER_SIZE;
    int virtual_address = -1;
    int base_address;
    int physical_address;
    char stored_value; 
    while (getline(&read_buffer, &size, fptr) != -1){
        virtual_address = atoi(read_buffer);
        read_page_info(&pifo, virtual_address);
        if(TBL_peek(tbl_cache, pifo.page_nr, &base_address) == 0){
            TBL_hit_count++;
            physical_address = base_address + pifo.offset;
            stored_value = PHYSICAL_MEMORY[physical_address];
            print_data(virtual_address, physical_address, stored_value);
            continue;
        }

        if (is_page_free(&pt, pifo.page_nr)){
            base_address = get_free_frame_baddr(list);
            page_fault_count++;
            if (base_address == -1) {
                printf("Error: Out of physical memory!\n");
                return -1;
            }
            set_page_base_addr(&pt, pifo.page_nr, base_address);
            read_disk(&disc, page_buffer, pifo.page_nr);
            write_frame(page_buffer, PHYSICAL_MEMORY ,base_address);
        }

        get_page_base_addr(&pt, pifo.page_nr, &base_address);
        TBL_enqueue (tbl_cache, pifo.page_nr, base_address);
        physical_address = base_address + pifo.offset;
        stored_value = PHYSICAL_MEMORY[physical_address];
        print_data(virtual_address, physical_address, stored_value);
    }
    printf("Page fault count: %d, TBL Hitrate: %d\n", page_fault_count, TBL_hit_count);

    return 0;
}