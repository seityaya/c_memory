//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#include "stdio.h"
#include "inttypes.h"
#include "malloc.h"
#include "stddef.h"
#include "string.h"

#include "yaya_memory.h"

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
    memset(&mem->memory_ptr[mem->memory_request], 0x88, produce - mem->memory_request - offsetof(mem_info_t, memory_ptr));

    return true;
}

void test_param() {
#if YAYA_MEMORY_STATS_USE
    mem_stats_t* mem_stats = NULL;
    if(memory_stats_init(&mem_stats)){
        printf("00 OK\n");
    }else{
        printf("ER\n");
    }
#endif

#if YAYA_MEMORY_STATS_USE
    if(!mem_new(mem_stats, NULL, NULL, 0)){
#else
    if(!mem_new(NULL, NULL, 0)){
#endif
        printf("01 OK\n");
    }else{
        printf("ER\n");
    }

    void *ptr = NULL;

#if YAYA_MEMORY_STATS_USE
    if(!mem_new(mem_stats, &ptr, NULL, 0)){
#else
    if(!mem_new(&ptr, NULL, 0)){
#endif
        printf("02 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE
    if(!mem_new(mem_stats, &ptr, ptr, 0)){
#else
    if(!mem_new(&ptr, ptr, 0)){
#endif
        printf("03 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE
    if(!mem_new(mem_stats, NULL, NULL, 1)){
#else
    if(!mem_new(NULL, NULL, 1)){
#endif
        printf("04 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE
    if(!mem_new(mem_stats, NULL, ptr, 1)){
#else
    if(!mem_new(NULL, ptr, 1)){
#endif
        printf("05 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE
    if(!mem_new(mem_stats, ptr, ptr, 1)){
#else
    if(!mem_new(ptr, ptr, 1)){
#endif
        printf("06 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE
    if(!mem_del(mem_stats, ptr)){
#else
    if(!mem_del(ptr)){
#endif
        printf("07 OK\n");
    }else{
        printf("ER\n");
    }
#if YAYA_MEMORY_STATS_USE
    if(!mem_del(mem_stats, &ptr)){
#else
    if(!mem_del(&ptr)){
#endif
        printf("08 OK\n");
    }else{
        printf("ER\n");
    }
#if YAYA_MEMORY_STATS_USE
    if(mem_new(mem_stats, &ptr, ptr, 1)){
#else
    if(mem_new(&ptr, ptr, 1)){
#endif
        printf("09 OK\n");
    }else{
        printf("ER\n");
    }
#if YAYA_MEMORY_STATS_USE
    if(mem_del(mem_stats, &ptr)){
#else
    if(mem_del(&ptr)){
#endif
        printf("10 OK\n");
    }else{
        printf("ER\n");
    }
#if YAYA_MEMORY_STATS_USE
    if(!mem_del(mem_stats, &ptr)){
#else
    if(!mem_del(&ptr)){
#endif
        printf("11 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE
    if(memory_stats_show(mem_stats)){
        printf("12 OK\n");
    }else{
        printf("ER\n");
    }


    if(memory_stats_free(&mem_stats)){
        printf("13 OK\n");
    }else{
        printf("ER\n");
    }

    if(mem_stats == NULL){
        printf("14 OK\n");
    }else{
        printf("ER\n");
    }
#endif

}

void test_dump(){
#if YAYA_MEMORY_STATS_USE
    mem_stats_t* mem_stats = NULL;
    if(!memory_stats_init(&mem_stats)){
        return;
    }
#endif

    void *ptr = NULL;
#if YAYA_MEMORY_STATS_USE
    mem_new(mem_stats, &ptr, ptr, 16);
#else
    mem_new(&ptr, ptr, 16);
#endif
    memory_dump(ptr, 0, 1, 16);

#if YAYA_MEMORY_STATS_USE
    mem_new(mem_stats, &ptr, ptr, 17);
#else
    mem_new(&ptr, ptr, 17);
#endif
    memory_dump(ptr, 0, 1, 16);

    printf("\n");

#if YAYA_MEMORY_STATS_USE
    mem_new(mem_stats, &ptr, ptr, 42);
#else
    mem_new(&ptr, ptr, 42);
#endif
    mem_dump(ptr);

    memory_fill(ptr);
    mem_dump(ptr);

#if YAYA_MEMORY_STATS_USE
    mem_new(mem_stats, &ptr, ptr, 35);
#else
    mem_new(&ptr, ptr, 35);
#endif
    mem_dump(ptr);

    memory_fill(ptr);
    mem_dump(ptr);

    printf("\n");

#if YAYA_MEMORY_STATS_USE
    mem_new(mem_stats, &ptr, ptr, 33);
#else
    mem_new(&ptr, ptr, 33);
#endif
    memory_fill(ptr);

    memory_dump(ptr, 0, 1, 4);
    memory_dump(ptr, 0, 1, 8);
    memory_dump(ptr, 0, 1, 16);
    memory_dump(ptr, 0, 1, 32);
    memory_dump(ptr, 0, 1, 64);

    void *ptr_save = ptr;
#if YAYA_MEMORY_STATS_USE
    mem_del(mem_stats, &ptr);
#else
    mem_del(&ptr);
#endif
    memory_dump(ptr_save, 33, 1, 64);

#if YAYA_MEMORY_STATS_USE
    memory_stats_show(mem_stats);
    memory_stats_free(&mem_stats);
#endif
}

int main()
{
    test_param();
    test_dump();

    return 0;
}
