//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_H
#define YAYA_MEMORY_H

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#ifndef YAYA_MEMORY_STATS_USE
#define YAYA_MEMORY_STATS_USE 0
#endif /*YAYA_MEMORY_STATS_USE*/

#ifndef YAYA_MEMORY_STATS_OFF
#if    !YAYA_MEMORY_STATS_USE
#define YAYA_MEMORY_STATS_OFF 0
#endif /*YAYA_MEMORY_STATS_USE*/
#endif /*YAYA_MEMORY_STATS_OFF*/

#ifndef YAYA_MEMORY_MACRO_DEF
#define YAYA_MEMORY_MACRO_DEF 0
#endif /*YAYA_MEMORY_MACRO_DEF*/

#ifndef YAYA_MEMORY_FILL_NULL_AFTER_FREE
#define YAYA_MEMORY_FILL_NULL_AFTER_FREE 1
#endif /*YAYA_MEMORY_FILL_NULL_AFTER_FREE*/

#if YAYA_MEMORY_STATS_USE
typedef struct mem_stats_t {
    intptr_t memory_request;  //запросил
    intptr_t memory_produce;  //выдали
    intptr_t memory_release;  //освободил
    intmax_t memory_call_new; //фактически выдано
    intmax_t memory_call_res; //фактически перераспределено
    intmax_t memory_call_del; //фактически удалено
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
    size_t memory_request;  //запросили
    size_t memory_produce;  //выдали
    uint8_t  memory_ptr[];  //указатель на начало
}mem_info_t;

#if YAYA_MEMORY_STATS_USE
bool   memory_new(mem_stats_t *mem_stats, void **ptr, void *old_ptr, const size_t new_size_len);
bool   memory_del(mem_stats_t *mem_stats, void **ptr);
#else
bool   memory_new(void **ptr, void *old_ptr, const size_t new_size_len);
bool   memory_del(void **ptr);
#endif /*YAYA_MEMORY_STATS_USE*/

bool   memory_zero(void *ptr);
size_t memory_size(void *ptr);
void   memory_swap(void *x, void *y, size_t size);
void   memory_shuf(void *base, size_t count, size_t size, unsigned int seed, void (*set_seed)(unsigned int), int (*get_rand)(void));
void   memory_sort(void *base, size_t count, size_t size, int (*compare)(const void *, const void *));
bool   memory_dump(void *ptr, size_t len, uintmax_t catbyte, uintmax_t column_mod2);
bool   memory_look(void *ptr, uintmax_t struct_count, size_t struct_size, intmax_t list_bit_len[]);

#if YAYA_MEMORY_MACRO_DEF
#if YAYA_MEMORY_STATS_USE
#define mem_new(S, N, O, s)      memory_new((S), (void**)(N), (void*)(O), (s))
#define mem_del(S, N)            memory_del((S), (void**)(N))
#else
#define mem_new(N, O, s)         memory_new((void**)(N), (void*)(O), (s))
#define mem_del(N)               memory_del((void**)(N))
#endif /*YAYA_MEMORY_STATS_USE*/

#define mem_zero(P)              memory_zero((void*)(P))
#define mem_size(P)              memory_size((void*)(P))
#define mem_swap(x, y, S)        memory_swap((void*)(x), (void*)(y), (S))
#define mem_shuf(P, C, S, seed)  memory_shuf((void*)(P), (C), (S), (seed), (NULL), (NULL))
#define mem_sort(P, C, S, Fcomp) memory_sort((void*)(P), (C), (S), (int(*) (const void *, const void *)) (Fcomp))
#define mem_dump(P)              memory_dump((void*)(P), 0, 1, 16)
#define mem_look(P, C, S, M)     memory_look((void*)(P), C, sizeof(S), M)
#define mem_list(...)            ({ (intmax_t[]){__VA_ARGS__, 0}; })

#endif /*YAYA_MEMORY_MACRO_DEF*/

#endif /*YAYA_MEMORY_H*/
