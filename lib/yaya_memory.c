//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#include "inttypes.h"
#include "malloc.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "yaya_memory.h"

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_OFF
bool memory_stats_init(mem_stats_t **mem_stats)
{
    if(mem_stats == NULL){
        return false;
    }

    if(*mem_stats == NULL){
        *mem_stats = (mem_stats_t*)malloc(sizeof(mem_stats_t));
        if(*mem_stats == NULL){
            return false;
        }

        memset(*mem_stats, 0, malloc_usable_size(*mem_stats));
        return true;
    }

    return false;
}

bool memory_stats_free(mem_stats_t **mem_stats)
{
    if(mem_stats != NULL){
        free(*mem_stats);
        *mem_stats = NULL;
        return true;
    }
    return false;
}

bool memory_stats_show(mem_stats_t *mem_stats)
{
    if(mem_stats != NULL){
        printf("Request :% 10" PRIiMAX "; ",   mem_stats->memory_request);
        printf("NEW  :% 10" PRIiMAX "; ",   mem_stats->memory_call_new);
        printf("\n");
        printf("Produce :% 10" PRIiMAX "; ",   mem_stats->memory_produce);
        printf("RES  :% 10" PRIiMAX "; ",   mem_stats->memory_call_res);
        printf("\n");
        printf("Overhead:% 10" PRIiMAX "; ",  mem_stats->memory_produce - mem_stats->memory_request);
        printf("DEL  :% 10" PRIiMAX "; ",   mem_stats->memory_call_del);
        printf("\n");
        printf("Release :% 10" PRIiMAX "; ",   mem_stats->memory_release);
        printf("USAGE:% 10" PRIiMAX "; ", mem_stats->memory_produce - mem_stats->memory_release);
        printf("\n");
        fflush(stdout);
        return true;
    }
    return false;
}
#endif

bool memory_new(
        #if YAYA_MEMORY_STATS_USE
        mem_stats_t *mem_stats,
        #endif
        void **ptr,
        void *old_ptr,
        const size_t new_size_len)
{
#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_OFF
    (void*)(mem_stats);
#endif

    /*Указатели под структуру памяти*/
    mem_info_t *mem_old = {0};
    mem_info_t *mem_new = {0};

    /*Проверка, что запрошено не нулевой размер памяти*/
    if(new_size_len == 0){
        return false;
    }

    /*Проверка, что возвращать есть куда*/
    if(ptr == NULL){
        return false;
    }

    /*Если память не инициализирована, то указатель на предыдущую память NULL*/
    if(old_ptr == NULL){
        /*Выделение памяти под запрос и на хранение информации и указателя*/
        mem_new = malloc(new_size_len + sizeof(mem_info_t));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL){
            return false;
        }

        size_t produce = malloc_usable_size(mem_new);

        /*Зануление всего выделеного*/
        memset(mem_new, 0x00, produce);

        /*Сохранение информации о количестве памяти*/
        mem_new->memory_request = (intptr_t)new_size_len;
        mem_new->memory_produce = (intptr_t)produce;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_OFF
        /*Сохранение статистики*/
        if(mem_stats != NULL){
            mem_stats->memory_call_new++;
            mem_stats->memory_request += mem_new->memory_request;
            mem_stats->memory_produce += mem_new->memory_produce;
        }
#endif
    }
    /*Если память инициализированна, то указатель на предыдущую память не NULL*/
    else
    {
        /*Помещаем указатель со смещением*/
        mem_old = old_ptr - offsetof(mem_info_t, memory_ptr);

        /*Запоминаем сколько было выделено и сколько запрошено*/
        intptr_t old_size_r = mem_old->memory_request;
#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_OFF
        intptr_t old_size_p = mem_old->memory_produce;
#endif
        /*Перераспределяем память*/
        mem_new = realloc(mem_old, new_size_len + sizeof(mem_info_t));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL){
            return false;
        }

        /*Запоминаем сколько выделено и сколько запрошено*/
        intptr_t new_size_p = (intptr_t)malloc_usable_size(mem_new);
        intptr_t new_size_r = (intptr_t)new_size_len;

        /*Вычисление разницы*/
        intptr_t diff_r = new_size_r - old_size_r;
#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_OFF
        intptr_t diff_p = new_size_p - old_size_p;
#endif
        /*Зануление хвоста выделеного*/
        if(diff_r > 0){
            intptr_t diff = new_size_p - (intptr_t)offsetof(mem_info_t, memory_ptr) - old_size_r;
            memset(mem_new->memory_ptr + old_size_r, 0x00, diff);
        }

        if(diff_r < 0){
            intptr_t diff = new_size_p - (intptr_t)offsetof(mem_info_t, memory_ptr) - new_size_r;
            memset(mem_new->memory_ptr + new_size_r, 0x00, diff);
        }

        /*Сохранение информации о количестве запрощеной памяти*/
        mem_new->memory_produce = new_size_p;
        mem_new->memory_request = new_size_r;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_OFF
        /*Сохранение статистики*/
        if(mem_stats != NULL){
            mem_stats->memory_call_res++;
            mem_stats->memory_request += diff_r;
            mem_stats->memory_produce += diff_p;
        }
#endif
    }
    return true;
}

bool memory_del(
        #if YAYA_MEMORY_STATS_USE
        mem_stats_t *mem_stats,
        #endif
        void **ptr)
{
#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_OFF
    (void*)(mem_stats);
#endif

    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    if(*ptr == NULL){
        return false;
    }

    /*Указатели под структуру памяти*/
    mem_info_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = *ptr - offsetof(mem_info_t, memory_ptr);

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_OFF
    /*Сохранение статистики*/
    if(mem_stats != NULL){
        mem_stats->memory_call_del++;
        mem_stats->memory_release += mem->memory_produce;
    }
#endif

#if YAYA_MEMORY_FILL_NULL_AFTER_FREE
    volatile uintptr_t size = mem->memory_produce;
    volatile uint8_t  *p    = (uint8_t*)(mem);
    while (size--){
        *p++ = 0;
    }
#endif

    free(mem);
    mem = NULL;
    *ptr = NULL;

    return true;
}

bool memory_zero(void *ptr)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    /*Указатель под структуру памяти*/
    mem_info_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(mem_info_t, memory_ptr);

    /*Сохраняем значения*/
    volatile intptr_t size = mem_info->memory_request;
    volatile uint8_t  *p   = mem_info->memory_ptr;

    /*Заполняем нулями*/
    while (size--){
        *p++ = 0;
    }

    return true;
}

size_t memory_size(void *ptr)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return 0;
    }

    /*Указатель под структуру памяти*/
    mem_info_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(mem_info_t, memory_ptr);

    /*Возвращаем размер запрошеной памяти*/
    return mem_info->memory_request;
}

void memory_swap(void *x, void *y, size_t size)
{
    char temp[size];
    memcpy(temp, y,    size);
    memcpy(y,    x,    size);
    memcpy(x,    temp, size);
}

void memory_shuf(void *base, size_t count, size_t size, unsigned int seed, void (*set_seed)(unsigned int), int (*get_rand)(void))
{
    if(get_rand == NULL){
        get_rand = (int (*)(void))(rand);
    }
    if(set_seed == NULL){
        set_seed = (void(*)(unsigned int))(srand);
    }
    set_seed(seed);
    for(size_t i = 0; i < count; i++){
        size_t r =  (((size_t)get_rand()) % (count - i + 1)) ;
        memory_swap(base + i * size, base + r * size, size);
    }
    for(size_t i = 0; i < count; i++){
        size_t a = (((size_t)get_rand()) % (count)) ;
        size_t b = (((size_t)get_rand()) % (count)) ;
        memory_swap(base + a * size, base + b * size, size);
    }
}

void memory_sort(void *base, size_t count, size_t size, int (*compare)(const void *, const void *))
{
    qsort(base, count, size, compare);
}

bool memory_dump(void *ptr, size_t len, uintmax_t catbyte, uintmax_t column)
{
    /*Проверка, что указатель не NULL*/
    if(ptr == NULL){
        return false;
    }

    /*Проверка, что числа есть степень 2*/
    if(catbyte == 0 || (catbyte & (catbyte - 1)) != 0){
        return false;
    }
    if(column == 0 || (column & (column - 1)) != 0){
        return false;
    }

    if(len == 0){
        mem_info_t *mem = NULL;
        mem = ptr - offsetof(mem_info_t, memory_ptr);
        ptr = mem->memory_ptr;
        len = mem->memory_request;
    }

    uintmax_t const col1 = (1 + 2 + 16 + 1);
    uintmax_t const col2 = (((column * catbyte) * 2) + column) + 1;

    /*Шапка*/
    {
        printf("╭");
        for(uintmax_t i = 0; i < col1; i++){
            printf("-");
        }
        printf("┬");
        for(uintmax_t i = 0; i < col2; i++){
            printf("-");
        }
        printf("╮");
        printf("\n");
        printf("| Len: %8"PRIuMAX" byte | ", len);
        for(uintmax_t i = 0; i < column; i++){
            for(uintmax_t j = 0; j < catbyte; j++){
                printf("%02"PRIXMAX"", i*catbyte+j);
            }
            printf(" ");
        }
        printf("|");
        printf("\n");
        printf("├");
        for(uintmax_t i = 0; i < col1; i++){
            printf("-");
        }
        printf("┼");
        for(uintmax_t i = 0; i < col2; i++){
            printf("-");
        }
        printf("┤");
        printf("\n");
    }

    /*Тело*/
    {
        uintptr_t madr = ((uintptr_t)ptr % (column * catbyte)) % 0x10;
        uint8_t *nadr = ptr - madr;

        for(uintmax_t m = 0; m < len; ){
            if(m == 0){
                printf("| 0x%016" PRIXPTR " | ", (uintptr_t)nadr);
            }else{
                printf("| 0x%016" PRIXPTR " | ", (uintptr_t)ptr + m);
            }

            for(uintmax_t i = 0; i < column; i++){
                uintmax_t j = 0;

                if(m == 0){
                    for(j = 0; j < catbyte; j++){
                        if(&nadr[i * catbyte + j] == ptr){
                            goto L1;
                        }
                        printf("..");
                    }
                    printf(" ");
                    continue;
                }
L1:
                for(uintmax_t l = j; l < catbyte; l++){
                    if(m < len){
                        printf("%02" PRIx8 "", ((uint8_t*)ptr)[m]);
                        m++;

                        if(m == len){
                            continue;
                        }
                    }else{
                        printf("..");
                    }
                }
                printf(" ");
            }
            printf("|");
            printf("\n");
        }
    }

    /*Подвал*/
    {
        printf("╰");
        for(uintmax_t i = 0; i < col1; i++){
            printf("-");
        }
        printf("┴");
        for(uintmax_t i = 0; i < col2; i++){
            printf("-");
        }
        printf("╯");
    }
    printf("\n");
    if(fflush(stdout) == 0){
        return true;
    }
    return true;
}

static uintmax_t bit_sequence(void *ptr, uintmax_t offset, uintmax_t len){
    uint8_t *bytes = (uint8_t*)ptr;
    uintmax_t result = 0;
    uint32_t result32_1 = 0;
    uint32_t result32_2 = 0;

    if(len == 0U){
        return result;
    }
    if(len <= 16U){
        for (uintmax_t i = 0U; i < len; i++) {
            result |= ((bytes[(offset + i) / 8U] >> ((offset + i) % 8U)) & 1U) << i;
        }
        return result;
    }
    if(len <= 32U){
        for (uintmax_t i = 0U; i < len; i++) {
            result32_1 |= ((bytes[(offset + i) / 8U] >> ((offset + i) % 8U)) & 1U) << i;
        }
        result = result32_1;
        return result;
    }
    if(len <= 64U){
        for (uintmax_t i = 0U; i < 32U; i++) {
            result32_1 |= ((bytes[(offset + i) / 8U] >> ((offset + i) % 8U)) & 1U) << i;
        }
        for (uintmax_t i = 0U; i < 64U - (len - 32U); i++) {
            result32_2 |= ((bytes[(offset + 32U + i) / 8U] >> ((offset + i) % 8U)) & 1U) << i;
        }
        result = (uintmax_t)result32_1 | ((uintmax_t)result32_2) << 32U;
        return result;
    }

    return result;
}

bool memory_look(void *ptr, uintmax_t struct_count, size_t struct_size, intmax_t list_bit_len[]){
    /*Проверка, что указатель не NULL*/
    if(ptr == NULL){
        return false;
    }

    /* Проверка и отладочная информация */
    {
        /* Объявление вычислимых констант */
        intmax_t list_bit_count = 0;
        intmax_t list_bit_sum   = 0;
        intmax_t list_str_sum   = 0;
        intmax_t list_str_count = 0;

        /* Вычисление констант */
        for(intmax_t i = 0; 0 == 0; i++){
            if(list_bit_len[i] == 0){
                list_bit_count = i;
                break;
            }
            if(list_bit_len[i] > 0){
                list_str_sum += list_bit_len[i];
                list_str_count++;
                list_bit_sum += list_bit_len[i];
            }else{
                list_bit_sum += -list_bit_len[i];
            }
        }

        /* Проверка, что переданый размер структуры и массив отступов совпадают */
        if((struct_size * __CHAR_BIT__) != list_bit_sum) {
            printf("ERROR:\n");
            printf("ptr = 0x%016" PRIXPTR "\n", (uintptr_t)((uint8_t*)(ptr)));
            printf("ucn = %ld\n", struct_count);
            printf("lcn = %ld\n", list_bit_count);
            printf("bit = %ld bit\n", struct_size * __CHAR_BIT__);
            printf("sum = %ld bit\n", list_bit_sum);
            printf("dlt = %ld\n", struct_size * __CHAR_BIT__ - list_bit_sum);
            printf("lbl = [");
            for(intmax_t i = 0; 0 == 0; i++){
                if(list_bit_len[i] == 0){
                    break;
                }
                printf("%" PRIiMAX "", list_bit_len[i]);
                if(list_bit_len[i+1] != 0){
                    printf(", ");
                }
            }
            printf("]\n");
        }
    }

    /* Таблица */
    {
        uintmax_t col1 = (1 + 2 + 16 + 1);
        uintmax_t col2 = 1;
        uintmax_t list_count = 0;
        uintmax_t bits_count = 0;

        for(intmax_t i = 0; 0 == 0; i++){
            if(list_bit_len[i] == 0){
                list_count = i;
                break;
            }
            if(list_bit_len[i] < 0){
                continue;
            }
            bits_count += list_bit_len[i];
            col2 += list_bit_len[i] / 4 + 1;
            if(list_bit_len[i] < 4 || list_bit_len[i] % 4 > 0){
                col2 += 1;
            }
        }

        /* Шапка */
        {
            printf("╭");
            for(uintmax_t i = 0; i < col1; i++){
                printf("-");
            }
            printf("┬");
            for(uintmax_t i = 0; i < col2; i++){
                printf("-");
            }
            printf("╮");
            printf("\n");
            printf("| S: %5"PRIuMAX"/%-5"PRIuMAX" bit | ", bits_count, struct_size * __CHAR_BIT__);
            for(uintmax_t i = 0; i < list_count; i++){
                if(list_bit_len[i] < 0){
                    continue;
                }
                printf("%*"PRIiMAX" ", (int)((list_bit_len[i] / 4) + ((list_bit_len[i] < 4 || list_bit_len[i] % 4 > 0) ? 1 : 0)), list_bit_len[i]);
            }
            printf("|");
            printf("\n");
            printf("├");
            for(uintmax_t i = 0; i < col1; i++){
                printf("-");
            }
            printf("┼");
            for(uintmax_t i = 0; i < col2; i++){
                printf("-");
            }
            printf("┤");
            printf("\n");
        }
        /* Тело */
        {
            for(uintmax_t i = 0; i < struct_count; i++){
                printf("| 0x%016" PRIXPTR " | ", (uintptr_t)ptr + (i * struct_size));
                intmax_t sum = 0;
                for(intmax_t s = 0; s < list_count; s++){
                    if(list_bit_len[s] > 0){
                        uint64_t res = bit_sequence(ptr, (struct_size * __CHAR_BIT__ * i) + sum, list_bit_len[s]);
                        printf("%0*" PRIx64 " ", (int)(list_bit_len[s] / 4 + ((list_bit_len[s] < 4 || list_bit_len[s] % 4 > 0) ? 1 : 0)), res);
                        sum += list_bit_len[s];
                    }else{
                        sum += -list_bit_len[s];
                    }
                }
                printf("|\n");
            }
        }
        /*Подвал*/
        {
            printf("╰");
            for(uintmax_t i = 0; i < col1; i++){
                printf("-");
            }
            printf("┴");
            for(uintmax_t i = 0; i < col2; i++){
                printf("-");
            }
            printf("╯");
        }
        printf("\n");
    }

    if(fflush(stdout) == 0){
        return true;
    }
    return true;
}
