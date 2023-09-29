//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_H
#define YAYA_MEMORY_H


#include "inttypes.h"
#include "malloc.h"
#include "stdalign.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


/*!
 * Включить подсчет статистики использования памяти глобально
 */
#ifndef YAYA_MEMORY_STATS_USE
#   define YAYA_MEMORY_STATS_USE        0
#endif /*YAYA_MEMORY_STATS_USE*/

#if YAYA_MEMORY_STATS_USE
/**
 * Включить подсчет статистики использования памяти локально
 */
#   ifndef YAYA_MEMORY_STATS_ON
#       define YAYA_MEMORY_STATS_ON     0
#   endif /*YAYA_MEMORY_STATS_ON*/

/**
 * Для подсчета статистики использовать глобальную переменную
 */
#   ifndef YAYA_MEMORY_STATS_GLOBAL
#       define YAYA_MEMORY_STATS_GLOBAL 0
#   endif /*YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/

/**
 * Включить использование сокращенных наименований функций
 */
#ifndef YAYA_MEMORY_MACRO_DEF
#   define YAYA_MEMORY_MACRO_DEF        0
#endif /*YAYA_MEMORY_MACRO_DEF*/

/**
 * Очищать область памяти перед освобождением
 */
#ifndef YAYA_MEMORY_FILL_BEFORE_FREE
#   define YAYA_MEMORY_FILL_BEFORE_FREE 1
#endif /*YAYA_MEMORY_FILL_BEFORE_FREE*/

/**
 * Заполнять память за пределами запрошеной области
 */
#ifndef YAYA_MEMORY_FILL_AFTER_MEM
#   define YAYA_MEMORY_FILL_AFTER_MEM   1
#endif /*YAYA_MEMORY_FILL_AFTER_MEM*/

/**
 * Специальное значение для заполнения за пределами запрошеной области
 */
#ifndef YAYA_MEMORY_VALUE_AFTER_MEM
#   define YAYA_MEMORY_VALUE_AFTER_MEM  0x00
#endif /*YAYA_MEMORY_VALUE_AFTER_MEM*/



#if YAYA_MEMORY_STATS_USE
typedef struct memory_stats_t {
    size_t memory_request;  //запросил
    size_t memory_produce;  //выдали
    size_t memory_release;  //освободил
    size_t memory_call_new; //фактически выдано
    size_t memory_call_res; //фактически перераспределено
    size_t memory_call_del; //фактически удалено
} memory_stats_t;

#   if YAYA_MEMORY_STATS_GLOBAL
extern memory_stats_t mem_stats_lib_global;
bool memory_stats_show();
#   else
#       if YAYA_MEMORY_STATS_ON
bool memory_stats_init(memory_stats_t **mem_stats);
bool memory_stats_free(memory_stats_t **mem_stats);
#       else
#define memory_stats_init(A) true
#define memory_stats_free(A) true
#       endif /*YAYA_MEMORY_STATS_ON*/
bool memory_stats_show(memory_stats_t  *mem_stats);
#   endif /*YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/



typedef struct memory_t {
    size_t memory_request;                       //запросили
    size_t memory_produce;                       //выдали
    alignas(max_align_t) uint8_t  memory_ptr[];  //указатель на начало
} memory_t;

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
bool   memory_new(memory_stats_t *mem_stats, void **ptr, void *old_ptr, const size_t count, const size_t size);
bool   memory_del(memory_stats_t *mem_stats, void **ptr);
#else
bool   memory_new(void **ptr, void *old_ptr, const size_t count, const size_t size);
bool   memory_del(void **ptr);
#endif /*YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL*/


typedef int  (*memory_func_comp_t)(const void *, const void *);
typedef void (*memory_func_seed_t)(unsigned int);
typedef int  (*memory_func_rand_t)(void);

bool   memory_zero(void *ptr);
bool   memory_fill(void *ptr, uint8_t value);
size_t memory_size(void *ptr);
bool   memory_dump(void *ptr, size_t len, uint64_t catbyte, uint64_t column_mod2);
bool   memory_look(void *ptr, size_t struct_count, size_t struct_size, int64_t list_bit_len[]);

bool   memory_swap(void *x, void *y, size_t size);
size_t memory_step(void *ptr_beg, void *ptr_end, size_t size);
bool   memory_shuf(void *base, size_t count, size_t size, unsigned int seed, memory_func_seed_t set_seed, memory_func_rand_t get_rand);
bool   memory_sort(void *base, size_t count, size_t size, memory_func_comp_t compare);
bool   memory_bsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare);
bool   memory_rsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare);

#define memory_list(...) ({ (int64_t[]){__VA_ARGS__, 0}; })

#endif /*YAYA_MEMORY_H*/
