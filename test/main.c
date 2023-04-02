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
#include "stdlib.h"
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
    printf("test_param\n");

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
    mem_stats_t* mem_stats = NULL;
    if(memory_stats_init(&mem_stats)){
        printf("00 OK\n");
    }else{
        printf("ER\n");
    }
#endif


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(mem_stats, NULL, NULL, 0, 0))
#else
    if(!memory_new(mem_stats, NULL, NULL, 0, 0))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(NULL, NULL, 0, 0))
#else
    if(!memory_new(NULL, NULL, 0, 0))
#endif
#endif
    {
        printf("01 OK\n");
    }else{
        printf("ER\n");
    }


    void *ptr = NULL;

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(mem_stats, &ptr, NULL, 0, 0))
#else
    if(!memory_new(mem_stats, &ptr, NULL, 0, 0))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(&ptr, NULL, 0, 0))
#else
    if(!memory_new(&ptr, NULL, 0, 0))
#endif
#endif
    {
        printf("02 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(mem_stats, &ptr, ptr, 0, 0))
#else
    if(!memory_new(mem_stats, &ptr, ptr, 0, 0))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(&ptr, ptr, 0, 0))
#else
    if(!memory_new(&ptr, ptr, 0, 0))
#endif
#endif
    {
        printf("03 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(mem_stats, NULL, NULL, 1, sizeof(char)))
#else
    if(!memory_new(mem_stats, NULL, NULL, 1, sizeof(char)))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(NULL, NULL, 1, sizeof(char)))
#else
    if(!memory_new(NULL, NULL, 1, sizeof(char)))
#endif
#endif
    {
        printf("04 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(mem_stats, NULL, ptr, 1, sizeof(char)))
#else
    if(!memory_new(mem_stats, NULL, ptr, 1, sizeof(char)))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(NULL, ptr, 1, sizeof(char)))
#else
    if(!memory_new(NULL, ptr, 1, sizeof(char)))
#endif
#endif
    {
        printf("05 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(mem_stats, ptr, ptr, 1, sizeof(char)))
#else
    if(!memory_new(mem_stats, ptr, ptr, 1, sizeof(char)))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_new(ptr, ptr, 1, sizeof(char)))
#else
    if(!memory_new(ptr, ptr, 1, sizeof(char)))
#endif
#endif
    {
        printf("06 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_del(mem_stats, ptr))
#else
    if(!memory_del(mem_stats, ptr))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_del(ptr))
#else
    if(!memory_del(ptr))
#endif
#endif
    {
        printf("07 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_del(mem_stats, &ptr))
#else
    if(!memory_del(mem_stats, &ptr))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_del(&ptr))
#else
    if(!memory_del(&ptr))
#endif
#endif
    {
        printf("08 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(mem_new(mem_stats, &ptr, ptr, 1, sizeof(char)))
#else
    if(memory_new(mem_stats, &ptr, ptr, 1, sizeof(char)))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(mem_new(&ptr, ptr, 1, sizeof(char)))
#else
    if(memory_new(&ptr, ptr, 1, sizeof(char)))
#endif
#endif
    {
        printf("09 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(mem_del(mem_stats, &ptr))
#else
    if(memory_del(mem_stats, &ptr))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(mem_del(&ptr))
#else
    if(memory_del(&ptr))
#endif
#endif
    {
        printf("10 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_del(mem_stats, &ptr))
#else
    if(!memory_del(mem_stats, &ptr))
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    if(!mem_del(&ptr))
#else
    if(!memory_del(&ptr))
#endif
#endif
    {
        printf("11 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_STATS_USE
#if YAYA_MEMORY_STATS_GLOBAL
    if(memory_stats_show()){
        printf("12 OK\n");
    }else{
        printf("ER\n");
    }
#else
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
#endif

    printf("\n");
    fflush(stdout);
}

void test_dump(){
    printf("test_dump\n");
#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
    mem_stats_t* mem_stats = NULL;
    if(!memory_stats_init(&mem_stats)){
        return;
    }
#endif

    void *ptr = NULL;

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(mem_stats, &ptr, ptr, 16, sizeof(char));
#else
    memory_new(mem_stats, &ptr, ptr, 16, sizeof(char));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&ptr, ptr, 16, sizeof(char));
#else
    memory_new(&ptr, ptr, 16, sizeof(char));
#endif
#endif

    memory_dump(ptr, 0, 1, 16);

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(mem_stats, &ptr, ptr, 17, sizeof(char));
#else
    memory_new(mem_stats, &ptr, ptr, 17, sizeof(char));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&ptr, ptr, 17, sizeof(char));
#else
    memory_new(&ptr, ptr, 17, sizeof(char));
#endif
#endif

    memory_dump(ptr, 0, 1, 16);

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(mem_stats, &ptr, ptr, 42, sizeof(char));
#else
    memory_new(mem_stats, &ptr, ptr, 42, sizeof(char));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&ptr, ptr, 42, sizeof(char));
#else
    memory_new(&ptr, ptr, 42, sizeof(char));
#endif
#endif

#if YAYA_MEMORY_MACRO_DEF
    mem_dump(ptr);
#else
    memory_dump(ptr, 0, 1, 16);
#endif

    memory_fill(ptr);

#if YAYA_MEMORY_MACRO_DEF
    mem_dump(ptr);
#else
    memory_dump(ptr, 0, 1, 16);
#endif

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(mem_stats, &ptr, ptr, 35, sizeof(char));
#else
    memory_new(mem_stats, &ptr, ptr, 35, sizeof(char));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&ptr, ptr, 35, sizeof(char));
#else
    memory_new(&ptr, ptr, 35, sizeof(char));
#endif
#endif

#if YAYA_MEMORY_MACRO_DEF
    mem_dump(ptr);
#else
    memory_dump(ptr, 0, 1, 16);
#endif

    memory_fill(ptr);

#if YAYA_MEMORY_MACRO_DEF
    mem_dump(ptr);
#else
    memory_dump(ptr, 0, 1, 16);
#endif

    printf("\n");

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(mem_stats, &ptr, ptr, 33, sizeof(char));
#else
    memory_new(mem_stats, &ptr, ptr, 33, sizeof(char));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&ptr, ptr, 33, sizeof(char));
#else
    memory_new(&ptr, ptr, 33, sizeof(char));
#endif
#endif

    memory_fill(ptr);

    memory_dump(ptr, 0, 1, 4);
    memory_dump(ptr, 0, 4, 2);
    memory_dump(ptr, 0, 4, 2);
    memory_dump(ptr, 0, 1, 8);
    memory_dump(ptr, 0, 1, 16);
    memory_dump(ptr, 0, 1, 32);
    memory_dump(ptr, 0, 1, 64);

    void *ptr_save = ptr;
#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_del(mem_stats, &ptr);
#else
    memory_del(mem_stats, &ptr);
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_del(&ptr);
#else
    memory_del(&ptr);
#endif
#endif

    memory_dump(ptr_save, 33, 1, 64);

#if YAYA_MEMORY_STATS_USE
#if YAYA_MEMORY_STATS_GLOBAL
    memory_stats_show();
#else
    memory_stats_show(mem_stats);
    memory_stats_free(&mem_stats);
#endif
#endif

    printf("\n");
    fflush(stdout);
}

void test_look(){
    printf("test_look\n");

    typedef struct S {
        uint8_t  x1:3;
        uint8_t  x2:1;
        uint8_t  x3:4;
        uint16_t a;
        int8_t   b;
        uint16_t c;
        uint8_t  d;
        int32_t  e;
        uint32_t f1:21;
        uint32_t f2:11;
        void*    p;
    } S;

    int a = 0;
    printf("%p\n", &a);

    S t[] = {
        {7, 1, 10, 5,  -1,       3,   1,   0, 17, 15, (int*)(3)},
        {7, 1, 10, 5,  -1,       2,   1,   1, 17, 15,        &a},
        {7, 1, 10, 5,  -1,       1,   1,  -1, 17, 15,        &t},
        {7, 1, 10, 5, 0-1,       0, 255,   2, 17, 15,        &t},
        {7, 1, 10, 5,   0,  0xFFFF,   0,  -2, 17, 15,   &t[0].a}
    };

    memory_dump(t, sizeof(S) * 5, 1, 16);

    memory_look(&t, 5, sizeof(S), ({ (intmax_t[]) { 3, 1, 4, 8, 16, 8, 8, 16, 8, 24, 32, 21, 11, 32, sizeof(void*) * __CHAR_BIT__, 0}; }) );
    memory_look(&t, 5, sizeof(S), mem_list(3, 1, 4, -8, 16, 8, 8, 16, 8, -24, 32, 21, 11, 32, 64));

#if YAYA_MEMORY_MACRO_DEF
    mem_look(&t, 5, S, mem_list(3, 1, 4, -8, 16, 8, 8, 16, 8, -24, 32, 21, 11, 32, 64));
#endif

    printf("\n");
    fflush(stdout);
}

void test_swap() {
    printf("test_swap\n");

    {
        int8_t x = 0;
        int8_t y = 1;


#if YAYA_MEMORY_MACRO_DEF
        mem_swap(&x, &y, sizeof(int8_t));
#else
        memory_swap(&x, &y, sizeof(int8_t));
#endif

        if(  1 == x &&  0 == y){
            printf("01 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        int16_t x = -1;
        int16_t y = +1;

#if YAYA_MEMORY_MACRO_DEF
        mem_swap(&x, &y, sizeof(int16_t));
#else
        memory_swap(&x, &y, sizeof(int16_t));
#endif

        if( +1 == x && -1 == y){
            printf("02 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        int32_t x = -136446856;
        int32_t y = +978321345;

#if YAYA_MEMORY_MACRO_DEF
        mem_swap(&x, &y, sizeof(int32_t));
#else
        memory_swap(&x, &y, sizeof(int32_t));
#endif

        if( +978321345 ==  x && -136446856 == y){
            printf("03 OK\n");
        }else{
            printf("ER\n");
        }
    }

    printf("\n");
    fflush(stdout);
}

void test_shuf() {
    printf("test_shuf\n");
    const int8_t count_mas = 10;
    const int8_t count_test = 127;
    int8_t *mas = NULL;

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(NULL, &mas, NULL, count_mas, sizeof(int8_t));
#else
    memory_new(NULL, (void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&mas, NULL, count_mas, sizeof(int8_t));
#else
    memory_new((void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));
#endif
#endif

    for(int8_t i = 0; i < count_mas; i++){
        mas[i] = i;
    }

    for(int8_t i = 0; i < count_mas; i++){
        printf("%"PRIi8 " ", mas[i]);
    }
    printf("\n");

    int32_t count_shuf = 0;
    for(int8_t a = 0; a < count_test; a++){
        for(int8_t i = 0; i < count_mas; i++){
            mas[i] = i;
        }

#if YAYA_MEMORY_MACRO_DEF
        mem_shuf(mas, count_mas, sizeof(int8_t), (uint)(a + 1));
#else
        memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), (uint)(a + 1), (void(*)(unsigned int))srand, (int(*)(void)) rand);
#endif

        for(int8_t i = 0; i < count_mas; i++){
            if(mas[i] != i){
                count_shuf++;
            }
        }

        for(int8_t i = 0; i < count_mas; i++){
            printf("%"PRIi8 " ", mas[i]);
        }
        printf("\n");
    }

    float mean = ((float)(count_shuf) / (float)(count_mas * count_test));

    if(mean > 0.8){
        printf("OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_del(NULL, &mas);
#else
    memory_del(NULL, (void**)(&mas));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_del(&mas);
#else
    memory_del((void**)(&mas));
#endif
#endif

    printf("\n");
    fflush(stdout);
}

static int comp (const int8_t *i, const int8_t *j) {
    return *i - *j;
}

void test_sort() {
    printf("test_sort\n");

    const int8_t count_mas = 10;
    int8_t *mas = {0};

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(NULL, &mas, NULL, count_mas, sizeof(int8_t));
#else
    memory_new(NULL, (void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&mas, NULL, count_mas, sizeof(int8_t));
#else
    memory_new((void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));
#endif
#endif

    for(int8_t i = 0; i < count_mas; i++){
        mas[i] = i;
    }

#if YAYA_MEMORY_MACRO_DEF
    mem_shuf(mas, count_mas, sizeof(int8_t), 1);
    mem_sort(mas, count_mas, sizeof(int8_t), comp);
#else
    memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), 1, (void(*)(unsigned int))srand, (int(*)(void)) rand);
    memory_sort(mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp));
#endif

    int8_t count_sort = 0;
    for(int8_t i = 0; i < count_mas; i++){
        if(mas[i] == i){
            count_sort++;
        }
    }

    if(count_mas == count_sort){
        printf("OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_del(NULL, &mas);
#else
    memory_del(NULL, (void**)(&mas));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_del(&mas);
#else
    memory_del((void**)(&mas));
#endif
#endif

    printf("\n");
    fflush(stdout);
}

void test_search() {
    printf("test_search\n");

    const int8_t count_mas = 10;

    int8_t *mas = NULL;
#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_new(NULL, &mas, NULL, count_mas, sizeof(int8_t));
#else
    memory_new(NULL, (void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_new(&mas, NULL, count_mas, sizeof(int8_t));
#else
    memory_new((void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));
#endif
#endif

    for(int8_t i = 0; i < count_mas; i++){
        mas[i] = i;
    }

    int8_t value = 11;
    int8_t *serch_r = NULL;
    int8_t *serch_b = NULL;

#if YAYA_MEMORY_MACRO_DEF
    if( ! mem_rsearch(&serch_r, &value, mas, count_mas, sizeof(int8_t), comp))
#else
    if( ! memory_rsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp)))
#endif
    {
        printf("1 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_MACRO_DEF
    if( ! mem_bsearch(&serch_b, &value, mas, count_mas, sizeof(int8_t), comp))
#else
    if( ! memory_bsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp)))
#endif
    {
        printf("2 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_r == NULL && serch_b == NULL){
        printf("3 OK\n");
    }else{
        printf("ER\n");
    }

    value = 9;
#if YAYA_MEMORY_MACRO_DEF
    if(mem_rsearch(&serch_r, &value, mas, count_mas, sizeof(int8_t), comp))
#else
    if(memory_rsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp)))
#endif
    {
        printf("4 OK\n");
    }else{
        printf("ER\n");
    }
#if YAYA_MEMORY_MACRO_DEF
    if(mem_bsearch(&serch_b, &value, mas, count_mas, sizeof(int8_t), comp))
#else
    if(memory_bsearch((void**)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp)))
#endif
    {
        printf("5 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_r == &mas[value] && serch_b == &mas[value]){
        printf("6 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_MACRO_DEF
    mem_shuf(mas, count_mas, sizeof(int8_t), 1);
#else
    memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), 1, NULL, NULL);
#endif

    for(int8_t i = 0; i < count_mas; i++){
        printf("%" PRIi8 " ", mas[i]);
    }
    printf("\n");
    int8_t position = 8;

#if YAYA_MEMORY_MACRO_DEF
    if(mem_rsearch(&serch_r, &value, mas, count_mas, sizeof(int8_t), comp))
#else
    if(memory_rsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp)))
#endif
    {
        printf("7 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_r == &mas[position]){
        printf("8 OK\n");
    }else{
        printf("ER\n");
    }


#if YAYA_MEMORY_MACRO_DEF
    if(mem_bsearch(&serch_b, &value, mas, count_mas, sizeof(int8_t), comp))
#else
    if(memory_bsearch((void**)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp)))
#endif
    {
        printf("8 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_b == &mas[position]){
        printf("9 OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_MACRO_DEF
    mem_sort(mas, count_mas, sizeof(int8_t), comp);
#else
    memory_sort(mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp));
#endif

#if YAYA_MEMORY_MACRO_DEF
    if(mem_bsearch(&serch_b, &value, mas, count_mas, sizeof(int8_t), comp))
#else
    if(memory_bsearch((void**)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (mem_compare_fn_t)(comp)))
#endif
    {
        printf("A OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_b == &mas[value]){
        printf("B OK\n");
    }else{
        printf("ER\n");
    }

    if(memory_step(mas, serch_b, sizeof(int8_t)) == value){
        printf("C OK\n");
    }else{
        printf("ER\n");
    }

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_MACRO_DEF
    mem_del(NULL, &mas);
#else
    memory_del(NULL, (void**)(&mas));
#endif
#else
#if YAYA_MEMORY_MACRO_DEF
    mem_del(&mas);
#else
    memory_del((void**)(&mas));
#endif
#endif

    printf("\n");
    fflush(stdout);
}

int main()
{
    test_param();
    test_dump();
    test_look();
    test_swap();
    test_shuf();
    test_sort();
    test_search();
    return 0;
}
