/*
 * Copyright 2023 Alexandre Fernandez <alex@fernandezfamily.email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include "zylib_def.h"

/*
 * Types
 */

typedef struct zylib_alloc_s zylib_alloc_t;
typedef void *(*zylib_malloc_t)(size_t size);
typedef void *(*zylib_realloc_t)(void *ptr, size_t size);
typedef void (*zylib_free_t)(void *ptr);

/*
 * Functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((nonnull)) zylib_return_t zylib_alloc_construct(zylib_alloc_t **alloc, zylib_malloc_t malloc,
                                                                  zylib_realloc_t realloc, zylib_free_t free);
    __attribute__((nonnull)) void zylib_alloc_destruct(zylib_alloc_t **alloc);
    __attribute__((nonnull)) zylib_return_t zylib_malloc(const zylib_alloc_t *alloc, size_t size, void **ptr);
    __attribute__((nonnull)) zylib_return_t zylib_realloc(const zylib_alloc_t *alloc, size_t size, void **ptr);
    __attribute__((nonnull)) void zylib_free(const zylib_alloc_t *alloc, void **ptr);

#ifdef __cplusplus
}
#endif