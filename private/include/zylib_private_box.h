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
#include "zylib_private_allocator.h"
#include <stdint.h>

/**
 * Box Data Structure
 */
typedef struct zylib_private_box_s zylib_private_box_t;

ZYLIB_BEGIN_DECLS

/**
 * Construct a box object
 * @param obj The object to construct
 * @param allocator The allocator object
 * @param size The size of the memory region
 * @param ptr The address of the memory region to copy
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_box_construct(zylib_private_box_t **obj, const zylib_private_allocator_t *allocator, uint64_t size,
                                  const void *ptr);

/**
 * Deconstruct a box object
 * @param obj The object to deconstruct
 */
ZYLIB_NONNULL
void zylib_private_box_destruct(zylib_private_box_t **obj);

/**
 * Append a memory region to a box object
 * @param obj The box object
 * @param size The size of the memory region
 * @param ptr The address of the memory region to copy
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_box_append(zylib_private_box_t **obj, uint64_t size, const void *ptr);

/**
 * Retrieve the memory region denoted that begins at index
 * @param obj The box object
 * @param index The index of the start of the memory region
 * @param size The pointer to the size of the memory region
 * @param ptr The pointer to the address of the memory region
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_box_split_by_index(const zylib_private_box_t *obj, uint64_t index, uint64_t *size,
                                       const void **ptr);

/**
 * Retrieve the size of the memory region that is stored at obj
 * @param obj The box object
 * @return The size of the memory region
 */
ZYLIB_NONNULL
uint64_t zylib_private_box_peek_size(const zylib_private_box_t *obj);

/**
 * Retrieve the address of the memory region that is stored at obj
 * @param obj The box object
 * @return The address of the memory region
 */
ZYLIB_NONNULL
const void *zylib_private_box_peek_data(const zylib_private_box_t *obj);

ZYLIB_END_DECLS
