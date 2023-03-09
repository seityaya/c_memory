//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_H
#define YAYA_MEMORY_H

#include "stdalign.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#ifndef YAYA_MEMORY_STATS_USE
#   define YAYA_MEMORY_STATS_USE 0
#endif /*YAYA_MEMORY_STATS_USE*/

#if !YAYA_MEMORY_STATS_USE
#   ifndef YAYA_MEMORY_STATS_OFF
#       define YAYA_MEMORY_STATS_OFF 0
#   endif /*YAYA_MEMORY_STATS_OFF*/

#   ifndef YAYA_MEMORY_STATS_GLOBAL
#       define YAYA_MEMORY_STATS_GLOBAL 0 /* TODO */
#   endif /*YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/

#ifndef YAYA_MEMORY_MACRO_DEF
#   define YAYA_MEMORY_MACRO_DEF 0
#endif /*YAYA_MEMORY_MACRO_DEF*/

#ifndef YAYA_MEMORY_FILL_NULL_AFTER_FREE
#   define YAYA_MEMORY_FILL_NULL_AFTER_FREE 1
#endif /*YAYA_MEMORY_FILL_NULL_AFTER_FREE*/

#ifndef YAYA_MEMORY_VALUE_AFTER_MEM
#   define YAYA_MEMORY_VALUE_AFTER_MEM 0x88
#endif /*YAYA_MEMORY_VALUE_AFTER_MEM*/

#if YAYA_MEMORY_STATS_USE
typedef struct mem_stats_t {
    size_t memory_request;  //запросил
    size_t memory_produce;  //выдали
    size_t memory_release;  //освободил
    size_t memory_call_new; //фактически выдано
    size_t memory_call_res; //фактически перераспределено
    size_t memory_call_del; //фактически удалено
}mem_stats_t;

#if YAYA_MEMORY_STATS_OFF
#define memory_stats_init(A) true
#define memory_stats_free(A) true
#define memory_stats_show(A) true
#else
bool memory_stats_init(mem_stats_t **mem_stats);
bool memory_stats_free(mem_stats_t **mem_stats);
bool memory_stats_show(mem_stats_t *mem_stats);
#endif /*YAYA_MEMORY_STATS_OFF*/
#endif /*YAYA_MEMORY_STATS_USE*/

typedef struct mem_info_t {
    size_t memory_request;                       //запросили
    size_t memory_produce;                       //выдали
    alignas(max_align_t) uint8_t  memory_ptr[];  //указатель на начало
}mem_info_t;

#if YAYA_MEMORY_STATS_USE
bool   memory_new(mem_stats_t *mem_stats, void **ptr, void *old_ptr, const size_t count, const size_t size);
bool   memory_del(mem_stats_t *mem_stats, void **ptr);
#else
bool   memory_new(void **ptr, void *old_ptr, const size_t count, const size_t size);
bool   memory_del(void **ptr);
#endif /*YAYA_MEMORY_STATS_USE*/

bool     memory_zero(void *ptr);
size_t   memory_size(void *ptr);
intmax_t memory_step(void *ptr_beg, void *ptr_bend, size_t size);

typedef int  (*mem_compare_fn_t)(const void *, const void *);
typedef int  (*mem_rand_fn_t)(void);
typedef void (*mem_seed_fn_t)(unsigned int);

bool memory_swap(void *x, void *y, size_t size);
bool memory_shuf(void *base, size_t count, size_t size, unsigned int seed, mem_seed_fn_t set_seed, mem_rand_fn_t get_rand);
bool memory_sort(void *base, size_t count, size_t size, mem_compare_fn_t compare);
bool memory_bsearch(void **search_res, void *key, void *base, size_t count, size_t size, mem_compare_fn_t compare);
bool memory_rsearch(void **search_res, void *key, void *base, size_t count, size_t size, mem_compare_fn_t compare);
bool memory_dump(void *ptr, size_t len, uintmax_t catbyte, uintmax_t column_mod2);
bool memory_look(void *ptr, size_t struct_count, size_t struct_size, intmax_t list_bit_len[]);
#define mem_list(...)                     ({ (intmax_t[]){__VA_ARGS__, 0}; })

#if YAYA_MEMORY_MACRO_DEF
#if YAYA_MEMORY_STATS_USE
#define mem_new(I, N, O, C, S)            memory_new((I), (void**)(N), (void*)(O), (size_t)(C), (size_t)(S))
#define mem_del(I, N)                     memory_del((I), (void**)(N))
#else
#define mem_new(N, O, C, S)               memory_new((void**)(N), (void*)(O), (size_t)(C), (size_t)(S))
#define mem_del(N)                        memory_del((void**)(N))
#endif /*YAYA_MEMORY_STATS_USE*/

#define mem_zero(P)                       memory_zero((void*)(P))
#define mem_size(P)                       memory_size((void*)(P))
#define mem_step(P, p, S)                 memory_step((void*)(P), (void*)(p), (size_t)(S))
#define mem_swap(x, y, S)                 memory_swap((void*)(x), (void*)(y), (size_t)(S))
#define mem_shuf(P, C, S, seed)           memory_shuf((void*)(P), (size_t)(C), (size_t)(S), (seed), (NULL), (NULL))
#define mem_sort(P, C, S, Fcomp)          memory_sort((void*)(P), (size_t)(C), (size_t)(S), (mem_compare_fn_t)(Fcomp))
#define mem_bsearch(R, K, P, C, S, Fcomp) memory_bsearch((void**)(R), (void*)(K), (void*)(P), (size_t)(C), (size_t)(S), (mem_compare_fn_t)(Fcomp))
#define mem_rsearch(R, K, P, C, S, Fcomp) memory_rsearch((void**)(R), (void*)(K), (void*)(P), (size_t)(C), (size_t)(S), (mem_compare_fn_t)(Fcomp))
#define mem_dump(P)                       memory_dump((void*)(P), 0, 1, 16)
#define mem_look(P, C, S, M)              memory_look((void*)(P), (size_t)(C), sizeof(S), M)
#endif /*YAYA_MEMORY_MACRO_DEF*/

#endif /*YAYA_MEMORY_H*/
