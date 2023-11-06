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
#include <stddef.h>

/*
 * Type Definitions
 */

typedef void *zylib_allocator_t;
typedef void *(*zylib_allocator_malloc_t)(size_t size);
typedef void *(*zylib_allocator_realloc_t)(void *ptr, size_t size);
typedef void (*zylib_allocator_free_t)(void *ptr);

/*
 * Function Declarations
 */

ZYLIB_BEGIN_DECLS

ZYLIB_NONNULL
_Bool zylib_allocator_construct(zylib_allocator_t **allocator, zylib_allocator_malloc_t malloc,
                                zylib_allocator_realloc_t realloc, zylib_allocator_free_t free);

ZYLIB_NONNULL
void zylib_allocator_destruct(zylib_allocator_t **allocator);

ZYLIB_NONNULL
_Bool zylib_allocator_malloc(const zylib_allocator_t *allocator, size_t size, void **data);

ZYLIB_NONNULL
_Bool zylib_allocator_realloc(const zylib_allocator_t *allocator, size_t size, void **data);

ZYLIB_NONNULL
void zylib_allocator_free(const zylib_allocator_t *allocator, void **data);

ZYLIB_END_DECLS
