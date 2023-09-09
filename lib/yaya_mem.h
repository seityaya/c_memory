//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.09
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEM_H
#define YAYA_MEM_H

#include "yaya_memory.h"

#   if YAYA_MEMORY_MACRO_DEF

#      if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#          define mem_new(I, N, O, C, S)    memory_new((I), (void**)(N), (void*)(O), (size_t)(C), (size_t)(S))
#          define mem_del(I, N)             memory_del((I), (void**)(N))
#      else
#          define mem_new(N, O, C, S)       memory_new((void**)(N), (void*)(O), (size_t)(C), (size_t)(S))
#          define mem_del(N)                memory_del((void**)(N))
#      endif /*YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL*/

#      define mem_zero(P)                   memory_zero((void*)(P))
#      define mem_size(P)                   memory_size((void*)(P))
#      define mem_fill(P, V)                memory_fill((void*)(P), V)
#      define mem_step(P, p, S)             memory_step((void*)(P), (void*)(p), (size_t)(S))
#      define mem_init(P, I, C)             memory_init((void*)(P), (void*)(I), (size_t)(C))
#      define mem_swap(x, y, S)             memory_swap((void*)(x), (void*)(y), (size_t)(S))
#      define mem_shuf(P, C, S, seed)       memory_shuf((void*)(P), (size_t)(C), (size_t)(S), (seed), (srand), (rand))
#      define mem_sort(P, C, S, F)          memory_sort((void*)(P), (size_t)(C), (size_t)(S), (mem_compare_fn_t)(F))
#      define mem_bsearch(R, K, P, C, S, F) memory_bsearch((void**)(R), (void*)(K), (void*)(P), (size_t)(C), (size_t)(S), (mem_compare_fn_t)(F))
#      define mem_rsearch(R, K, P, C, S, F) memory_rsearch((void**)(R), (void*)(K), (void*)(P), (size_t)(C), (size_t)(S), (mem_compare_fn_t)(F))
#      define mem_dump(P)                   memory_dump((void*)(P), 0, 1, 16)
#      define mem_look(P, C, S, M)          memory_look((void*)(P), (size_t)(C), (size_t)(S), M)

#   endif /*YAYA_MEMORY_MACRO_DEF*/

#endif /*YAYA_MEM_H*/
