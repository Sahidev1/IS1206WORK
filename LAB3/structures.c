#include "structures.h"
#include <stdio.h>

void init_page_table(page_table *pt){
    for(int i = 0; i < PAGE_TABLE_SIZE; i++){
        pt->page_array[i] = 256*i;
    }
    //pt->page_array[PAGE_TABLE_SIZE - 1] -= 1;
}

int get_page_base_addr (page_table *pt, int pgnr, int *base_addr_ptr){
    if (pgnr >= PAGE_TABLE_SIZE) return -1;
    *base_addr_ptr = pt->page_array[pgnr];
    return 0;
}

int find_base_addr (page_table *pt, int base_addr){
    for (int i = 0; i < PAGE_TABLE_SIZE; i++){
        if (pt->page_array[i] == base_addr){
            return i;
        }
    }
    return -1;
}