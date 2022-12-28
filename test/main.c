//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2022 Seityagiya Terlekchi. All rights reserved.

#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "stdio.h"
#include "inttypes.h"
#include "stddef.h"


#define USE_MEMORY_STATS 0
#include "yaya_memory.h"


bool memory_fill(void *ptr);

bool memory_fill(void *ptr){
    /*Проверка, что указатель не NULL*/
    if(ptr == NULL){
        printf("ERROR *POINTER\n"); fflush(stdout);
        return false;
    }

    /*Указатели под структуру памяти*/
    mem_info_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = ptr - offsetof(mem_info_t, memory_ptr);

    size_t produce = malloc_usable_size(mem);

    /*Заполнение памяти*/
    memset(mem->memory_ptr, 0x11, produce - offsetof(mem_info_t, memory_ptr));
    memset(mem->memory_ptr, 0xff, 1);
    memset(&mem->memory_ptr[mem->memory_request - 1], 0xff, 1);
    memset(&mem->memory_ptr[produce - offsetof(mem_info_t, memory_ptr) - 1], 0x88, 1);

    return true;
}

void test_param() {
    if(!mem_new(NULL, NULL, 0)){
        printf("01 OK\n");
    }else{
        printf("ER\n");
    }

    void *ptr = NULL;

    if(!mem_new(&ptr, NULL, 0)){
        printf("02 OK\n");
    }else{
        printf("ER\n");
    }

    if(!mem_new(&ptr, ptr, 0)){
        printf("03 OK\n");
    }else{
        printf("ER\n");
    }

    if(!mem_new(NULL, NULL, 1)){
        printf("04 OK\n");
    }else{
        printf("ER\n");
    }

    if(!mem_new(NULL, ptr, 1)){
        printf("05 OK\n");
    }else{
        printf("ER\n");
    }

    if(!mem_new(ptr, ptr, 1)){
        printf("06 OK\n");
    }else{
        printf("ER\n");
    }

    if(!mem_del(ptr)){
        printf("07 OK\n");
    }else{
        printf("ER\n");
    }

    if(!mem_del(&ptr)){
        printf("08 OK\n");
    }else{
        printf("ER\n");
    }

    if(mem_new(&ptr, ptr, 1)){
        printf("09 OK\n");
    }else{
        printf("ER\n");
    }

    if(mem_del(&ptr)){
        printf("10 OK\n");
    }else{
        printf("ER\n");
    }

    if(!mem_del(&ptr)){
        printf("11 OK\n");
    }else{
        printf("ER\n");
    }
}

void test_dump(){
    void *ptr = NULL;

    mem_new(&ptr, ptr, 16);
    memory_dump(ptr, 0, 1, 16);

    mem_new(&ptr, ptr, 17);
    memory_dump(ptr, 0, 1, 16);

    mem_new(&ptr, ptr, 33);
    memory_fill(ptr);
    memory_dump(ptr, 0, 4, 4);

    mem_new(&ptr, ptr, 30);
    mem_dump(ptr);

    memory_fill(ptr);
    mem_dump(ptr);
}

int main() {
    test_param();
    test_dump();

    return 0;
}
