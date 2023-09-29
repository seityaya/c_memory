//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#include "yaya_memory.h"

#if YAYA_MEMORY_STATS_USE
#if YAYA_MEMORY_STATS_GLOBAL
memory_stats_t mem_stats_lib_global = {0};
#endif /*YAYA_MEMORY_STATS_GLOBAL*/

bool memory_stats_show(
        #if !YAYA_MEMORY_STATS_GLOBAL
        memory_stats_t *mem_stats
        #endif
        )
{
#if YAYA_MEMORY_STATS_GLOBAL
    memory_stats_t *mem_stats = &mem_stats_lib_global;
#endif /*YAYA_MEMORY_STATS_GLOBAL*/

    if(mem_stats != NULL){
        printf("Request :% 10" PRIiMAX "; ", mem_stats->memory_request);
        printf("NEW  :% 10" PRIiMAX "; ",    mem_stats->memory_call_new);
        printf("\n");
        printf("Produce :% 10" PRIiMAX "; ", mem_stats->memory_produce);
        printf("RES  :% 10" PRIiMAX "; ",    mem_stats->memory_call_res);
        printf("\n");
        printf("Overhead:% 10" PRIiMAX "; ", mem_stats->memory_produce - mem_stats->memory_request);
        printf("DEL  :% 10" PRIiMAX "; ",    mem_stats->memory_call_del);
        printf("\n");
        printf("Release :% 10" PRIiMAX "; ", mem_stats->memory_release);
        printf("USAGE:% 10" PRIiMAX "; ",    mem_stats->memory_produce - mem_stats->memory_release);
        printf("\n");

        if(fflush(stdout) == 0){
            return true;
        }
    }
    return false;
}

#if YAYA_MEMORY_STATS_ON && !YAYA_MEMORY_STATS_GLOBAL
bool memory_stats_init(memory_stats_t **mem_stats)
{
    if(mem_stats == NULL){
        return false;
    }

    if(*mem_stats == NULL){
        *mem_stats = (memory_stats_t*)malloc(sizeof(memory_stats_t));
        if(*mem_stats == NULL){
            return false;
        }

        memset(*mem_stats, 0, malloc_usable_size(*mem_stats));
        return true;
    }

    return false;
}

bool memory_stats_free(memory_stats_t **mem_stats)
{
    if(mem_stats != NULL){
        free(*mem_stats);
        *mem_stats = NULL;
        return true;
    }
    return false;
}
#endif /*!YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/

bool memory_new(
        #if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
        memory_stats_t *mem_stats,
        #endif
        void **ptr,
        void *old_ptr,
        const size_t count,
        const size_t size)
{
#if YAYA_MEMORY_STATS_USE
#if YAYA_MEMORY_STATS_GLOBAL
#if YAYA_MEMORY_STATS_ON
    memory_stats_t *mem_stats = &mem_stats_lib_global;
#endif /*YAYA_MEMORY_STATS_ON*/
#else
    (void)(mem_stats);
#endif /*YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/

    /*Указатели под структуру памяти*/
    memory_t *mem_old = NULL;
    memory_t *mem_new = NULL;

    /*Проверка, что запрошено не нулевой размер памяти*/
    const size_t new_size_len = (count * size);
    if(new_size_len == 0){
        return false;
    }

    /*Проверка, что возвращать есть куда*/
    if(ptr == NULL){
        return false;
    }

    /*Если память не инициализирована, то указатель на предыдущую память NULL*/
    if(old_ptr == NULL){
        /*Выделение памяти на хранение информации и указателя*/
        mem_new = malloc(new_size_len + sizeof(memory_t));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL){
            return false;
        }

        size_t produce = malloc_usable_size(mem_new);

        /*Зануление всего выделенного*/
        memset(mem_new, 0x00, produce);

#if YAYA_MEMORY_FILL_AFTER_MEM
        /*Установка значения сверх выделенного*/
        memset(mem_new->memory_ptr + new_size_len, YAYA_MEMORY_VALUE_AFTER_MEM, produce - new_size_len - sizeof(memory_t));
#endif

        /*Сохранение информации о количестве памяти*/
        mem_new->memory_request = new_size_len;
        mem_new->memory_produce = produce;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_ON
        /*Сохранение статистики*/
        if(mem_stats != NULL){
            mem_stats->memory_call_new++;
            mem_stats->memory_request += mem_new->memory_request;
            mem_stats->memory_produce += mem_new->memory_produce - sizeof(memory_t);
        }
#endif
    }
    /*Если память инициализированна, то указатель на предыдущую память не NULL*/
    else
    {
        /*Помещаем указатель со смещением*/
        mem_old = old_ptr - sizeof(memory_t);

        /*Запоминаем сколько было выделено и сколько запрошено*/
        size_t old_size_r = mem_old->memory_request;
#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_ON
        size_t old_size_p = mem_old->memory_produce;
#endif
        /*Перераспределяем память*/
        mem_new = realloc(mem_old, new_size_len + sizeof(memory_t));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL){
            return false;
        }

        /*Запоминаем сколько выделено и сколько запрошено*/
        size_t new_size_p = malloc_usable_size(mem_new);
        size_t new_size_r = new_size_len;

        /*Вычисление разницы*/
        intptr_t diff_r = (intptr_t)(new_size_r) - (intptr_t)(old_size_r);
#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_ON
        intptr_t diff_p = (intptr_t)(new_size_p) - (intptr_t)(old_size_p);
#endif
        /*Зануление хвоста нового выделенного*/
        if(diff_r > 0){
            memset(mem_new->memory_ptr + old_size_r, 0x00, (size_t)(diff_r));
        }
#if YAYA_MEMORY_FILL_AFTER_MEM
        /*Установка значения сверх выделенного*/
        if(diff_r < 0){
            memset(mem_new->memory_ptr + new_size_r, YAYA_MEMORY_VALUE_AFTER_MEM, new_size_p - new_size_r - sizeof(memory_t));
        }
#endif

        /*Сохранение информации о количестве запрощеной памяти*/
        mem_new->memory_produce = new_size_p;
        mem_new->memory_request = new_size_r;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_ON
        /*Сохранение статистики*/
        if(mem_stats != NULL){
            mem_stats->memory_call_res++;
            mem_stats->memory_request += (size_t)(diff_r);
            mem_stats->memory_produce += (size_t)(diff_p);
        }
#endif
    }
    return true;
}

bool memory_del(
        #if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
        memory_stats_t *mem_stats,
        #endif
        void **ptr)
{
#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_ON
#if YAYA_MEMORY_STATS_GLOBAL
    memory_stats_t *mem_stats = &mem_stats_lib_global;
#else
    (void)(mem_stats);
#endif /*YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/

    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    if(*ptr == NULL){
        return false;
    }

    /*Указатели под структуру памяти*/
    memory_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = *ptr - offsetof(memory_t, memory_ptr);

#if YAYA_MEMORY_STATS_USE && YAYA_MEMORY_STATS_ON
    /*Сохранение статистики*/
    if(mem_stats != NULL){
        mem_stats->memory_call_del++;
        mem_stats->memory_release += mem->memory_produce;
    }
#endif

#if YAYA_MEMORY_FILL_BEFORE_FREE
    volatile size_t  volsize = mem->memory_produce;
    volatile uint8_t *volptr = (uint8_t*)(mem);
    while (volsize--){
        *(volptr++) = 0x00;
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
    memory_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(memory_t, memory_ptr);

    /*Сохраняем значения*/
    volatile size_t  volsize = mem_info->memory_request;
    volatile uint8_t *volptr = mem_info->memory_ptr;
    /*Заполняем нулями*/
    while (volsize--){
        *volptr++ = 0;
    }

    return true;
}

bool memory_fill(void *ptr, uint8_t value)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    /*Указатель под структуру памяти*/
    memory_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(memory_t, memory_ptr);

    /*Сохраняем значения*/
    volatile size_t  volsize = mem_info->memory_request;
    volatile uint8_t *volptr = mem_info->memory_ptr;
    /*Заполняем значением*/
    while (volsize--){
        *volptr++ = value;
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
    memory_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(memory_t, memory_ptr);

    /*Возвращаем размер запрошеной памяти*/
    return mem_info->memory_request;
}

bool memory_swap(void *swapA, void *swapB, size_t size)
{
    /*Проверка, что указатели не NULL*/
    if(swapA == NULL){
        return false;
    }
    if(swapB == NULL){
        return false;
    }
    if(size == 0){
        return false;
    }

    char tempX[size];
    memcpy(tempX, swapB, size);
    memcpy(swapB, swapA, size);
    memcpy(swapA, tempX, size);

    return true;
}

size_t memory_step(void* ptr_beg, void* ptr_end, size_t size)
{
    /*Проверка, что указатели не NULL*/
    if(ptr_beg == NULL){
        return 0;
    }
    if(ptr_end == NULL){
        return 0;
    }
    if(size == 0){
        return 0;
    }

    ptrdiff_t dist = (ptrdiff_t)(ptr_end) - (ptrdiff_t)(ptr_beg);

    if(dist % (ptrdiff_t)(size) != 0){
        return 0;
    }

    return (size_t)(dist / (ptrdiff_t)(size));
}

bool memory_shuf(void *base, size_t count, size_t size, unsigned int seed, memory_func_seed_t set_seed, memory_func_rand_t get_rand)
{
    /*Проверка, что указатели не NULL*/
    if(base == NULL){
        return false;
    }
    if(count == 0){
        return false;
    }
    if(size == 0){
        return false;
    }

    set_seed(seed);

    for(size_t i = 1; i < count; i++){
        size_t rndx =  ((((size_t)get_rand()) % (count - i)));
        memory_swap(base + i * size, base + rndx * size, size);
    }
    for(size_t i = 0; i < count; i++){
        size_t rnda = (((size_t)get_rand()) % (count)) ;
        size_t rndb = (((size_t)get_rand()) % (count)) ;
        memory_swap(base + rnda * size, base + rndb * size, size);
    }
    return true;
}

bool memory_sort(void *base, size_t count, size_t size, memory_func_comp_t compare)
{
    /*Проверка, что указатели не NULL*/
    if(base == NULL){
        return false;
    }
    if(compare == NULL){
        return false;
    }
    if(count == 0){
        return false;
    }
    if(size == 0){
        return false;
    }

    qsort(base, count, size, compare);

    return true;
}

bool memory_bsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare)
{
    /*Проверка, что указатели не NULL*/
    if(search_res == NULL){
        return false;
    }
    if(key == NULL){
        return false;
    }
    if(base == NULL){
        return false;
    }
    if(compare == NULL){
        return false;
    }
    if(size == 0){
        return false;
    }

    *search_res = bsearch(key, base, count, size, compare);
    if(*search_res != NULL){
        return true;
    }
    *search_res = NULL;
    return false;
}

bool memory_rsearch(void** search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare)
{
    /*Проверка, что указатели не NULL*/
    if(search_res == NULL){
        return false;
    }
    if(key == NULL){
        return false;
    }
    if(base == NULL){
        return false;
    }
    if(compare == NULL){
        return false;
    }
    if(size == 0){
        return false;
    }

    for(size_t i = 0; i < count; i++){
        if(compare(base + (i * size), key) == 0){
            *search_res =  base + (i * size);
            return true;
        }
    }
    *search_res = NULL;
    return false;
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
        memory_t *mem = ptr - offsetof(memory_t, memory_ptr);
        ptr = mem->memory_ptr;
        len = mem->memory_request;
    }

    uintmax_t const col1 = (1 + 2 + sizeof(uintptr_t) * 2 + 1);
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
        printf("| Len: %8" PRIuMAX " byte | ", len);
        for(uintmax_t i = 0; i < column; i++){
            for(uintmax_t j = 0; j < catbyte; j++){
                printf("%02" PRIXMAX "", i*catbyte+j);
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
        uint8_t *nadr = ptr - (((uintptr_t)ptr % (column * catbyte)) % 0x10);

        uintmax_t pos = 0;
        while(pos < len){
            if(pos == 0){
                printf("| 0x%0*" PRIXPTR " | ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)nadr);
            }else{
                printf("| 0x%0*" PRIXPTR " | ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)ptr + pos);
            }

            for(uintmax_t i = 0; i < column; i++){
                uintmax_t ind = 0;

                if(pos == 0){
                    for(ind = 0; ind < catbyte; ind++){
                        if(&nadr[i * catbyte + ind] == ptr){
                            goto L1;
                        }
                        printf("..");
                    }
                    printf(" ");
                    continue;
                }
L1:
                for(uintmax_t k = ind; k < catbyte; k++){
                    if(pos < len){
                        printf("%02" PRIx8 "", ((uint8_t*)ptr)[pos]);
                        pos++;

                        if(pos == len){
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
    return false;
}

static uint64_t bit_sequence(void *ptr, uintmax_t offset, uintmax_t len)
{
    uint8_t* bytes      = (uint8_t*)ptr;
    uint64_t result     = 0;
    uint32_t result32_1 = 0;
    uint32_t result32_2 = 0;

    if(len == 0){
        return 0;
    }

#define CU(x) (x##U)

    if(len <= CU(16)){
        for (uint64_t i = 0U; i < len; i++) {
            result |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        return result;
    }
    if(len <= CU(32)){
        for (uint64_t i = 0U; i < len; i++) {
            result32_1 |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        result = result32_1;
        return result;
    }
    if(len <= CU(64)){
        for (uint64_t i = 0U; i < CU(32); i++) {
            result32_1 |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        for (uint64_t i = 0U; i < CU(64) - (len - CU(32)); i++) {
            result32_2 |= ((uint64_t)(bytes[(offset + CU(32) + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        result = (uint64_t)result32_1 | ((uint64_t)result32_2) << CU(32);
        return result;
    }

#undef CU

    return 0;
}

bool memory_look(void *ptr, size_t struct_count, size_t struct_size, intmax_t list_bit_len[])
{
    /*Проверка, что указатель не NULL*/
    if(ptr == NULL){
        return false;
    }

    /* Проверка и отладочная информация */
    {
        /* Объявление вычислимых констант */
        uintmax_t list_bit_count = 0;
        uintmax_t list_bit_sum   = 0;

        /* Вычисление констант */
        for(uintmax_t i = 0; 0 == 0; i++){
            if(list_bit_len[i] == 0){
                list_bit_count = i;
                break;
            }
            if(list_bit_len[i] > 0){
                list_bit_sum += (uintmax_t)(+list_bit_len[i]);
            }else{
                list_bit_sum += (uintmax_t)(-list_bit_len[i]);
            }
        }

        /* Проверка, что переданый размер структуры и массив отступов совпадают */
        if((struct_size * __CHAR_BIT__) != list_bit_sum) {
            printf("ERROR: (struct_size * __CHAR_BIT__) != list_bit_sum \n");
            printf("ptr = 0x%0*" PRIXPTR "\n", (int)(sizeof(uintptr_t) * 2), (uintptr_t)((uint8_t*)(ptr)));
            printf("ucn = %" PRIiMAX "\n", struct_count);
            printf("lcn = %" PRIiMAX "\n", list_bit_count);
            printf("bit = %" PRIiMAX " bit\n", struct_size * __CHAR_BIT__);
            printf("sum = %" PRIiMAX " bit\n", list_bit_sum);
            printf("dlt = %" PRIiMAX "\n", struct_size * __CHAR_BIT__ - list_bit_sum);
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
        intmax_t col1 = (1 + 2 + (int)(sizeof(uintptr_t) * 2) + 1);
        intmax_t col2 = 1;
        intmax_t list_count = 0;
        intmax_t bits_count = 0;

        for(intmax_t i = 0; ; i++){
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
            for(intmax_t i = 0; i < col1; i++){
                printf("-");
            }
            printf("┬");
            for(intmax_t i = 0; i < col2; i++){
                printf("-");
            }
            printf("╮");
            printf("\n");
            printf("| S: %5" PRIuMAX "/%-5"PRIuMAX" bit | ", bits_count, struct_size * __CHAR_BIT__);
            for(intmax_t i = 0; i < list_count; i++){
                if(list_bit_len[i] < 0){
                    continue;
                }
                printf("%*"PRIiMAX" ", (int)((list_bit_len[i] / 4) + ((list_bit_len[i] < 4 || list_bit_len[i] % 4 > 0) ? 1 : 0)), list_bit_len[i]);
            }
            printf("|");
            printf("\n");
            printf("├");
            for(intmax_t i = 0; i < col1; i++){
                printf("-");
            }
            printf("┼");
            for(intmax_t i = 0; i < col2; i++){
                printf("-");
            }
            printf("┤");
            printf("\n");
        }
        /* Тело */
        {
            for(uintmax_t i = 0; i < struct_count; i++){
                printf("| 0x%0*" PRIXPTR " | ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)(ptr) + (i * (uintptr_t)(struct_size)));
                intmax_t offset = 0;
                for(intmax_t k = 0; k < list_count; k++){
                    if(list_bit_len[k] > 0){
                        uintmax_t res = bit_sequence(ptr, (uintptr_t)(struct_size * __CHAR_BIT__ * i) + (uintptr_t)(offset), (uintmax_t)(list_bit_len[k]));
                        printf("%0*" PRIxMAX " ", (int)(list_bit_len[k] / 4 + ((list_bit_len[k] < 4 || list_bit_len[k] % 4 > 0) ? 1 : 0)), res);
                        offset += list_bit_len[k];
                    }else{
                        offset += -list_bit_len[k];
                    }
                }
                printf("|\n");
            }
        }
        /*Подвал*/
        {
            printf("╰");
            for(intmax_t i = 0; i < col1; i++){
                printf("-");
            }
            printf("┴");
            for(intmax_t i = 0; i < col2; i++){
                printf("-");
            }
            printf("╯");
        }
        printf("\n");
    }

    if(fflush(stdout) == 0){
        return true;
    }
    return false;
}
