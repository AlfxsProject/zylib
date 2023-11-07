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

/**
 * Allocator Data Structure
 */
typedef struct zylib_private_allocator_s zylib_private_allocator_t;

/**
 * Malloc Function Pointer Data Type
 */
typedef void *(*zylib_private_allocator_malloc_t)(size_t size);

/**
 * Realloc Function Pointer Data Type
 */
typedef void *(*zylib_private_allocator_realloc_t)(void *ptr, size_t size);

/**
 * Free Function Pointer Data Type
 */
typedef void (*zylib_private_allocator_free_t)(void *ptr);

ZYLIB_BEGIN_DECLS

/**
 * Construct an allocator object
 * @param obj The object to construct
 * @param malloc The malloc function pointer
 * @param realloc The realloc function pointer
 * @param free The free function pointer
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_allocator_construct(zylib_private_allocator_t **obj, zylib_private_allocator_malloc_t malloc,
                                        zylib_private_allocator_realloc_t realloc, zylib_private_allocator_free_t free);

/**
 * Deconstruct an allocator object
 * @param obj The object to deconstruct
 */
ZYLIB_NONNULL
void zylib_private_allocator_destruct(zylib_private_allocator_t **obj);

/**
 * Allocate a memory region
 * @param obj The allocator object
 * @param size The size of the region to allocate
 * @param ptr The pointer to the address of the memory region
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_allocator_malloc(const zylib_private_allocator_t *obj, size_t size, void **ptr);

/**
 * Resize an allocated memory region
 * @param obj The allocator object
 * @param size The new size of the memory region
 * @param ptr The pointer to the address of the memory region
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_allocator_realloc(const zylib_private_allocator_t *obj, size_t size, void **ptr);

/**
 * Deallocate a memory region
 * @param obj The allocator object
 * @param ptr The pointer to the memory region
 */
ZYLIB_NONNULL
void zylib_private_allocator_free(const zylib_private_allocator_t *obj, void **ptr);

ZYLIB_END_DECLS
