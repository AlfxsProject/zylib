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
 * Double-Ended Queue Data Structure
 */
typedef struct zylib_private_dequeue_s zylib_private_dequeue_t;

ZYLIB_BEGIN_DECLS

/**
 * Construct a dequeue object
 * @param obj The object to construct
 * @param allocator The allocator object
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_dequeue_construct(zylib_private_dequeue_t **obj, const zylib_private_allocator_t *allocator);

/**
 * Deconstruct a dequeue object
 * @param obj The object to deconstruct
 */
ZYLIB_NONNULL
void zylib_private_dequeue_destruct(zylib_private_dequeue_t **obj);

/**
 * Deconstruct all nodes contained within a dequeue
 * @param obj The dequeue object
 */
ZYLIB_NONNULL
void zylib_private_dequeue_clear(zylib_private_dequeue_t *obj);

/**
 * Insert a node at the beginning of a dequeue
 * @param obj The dequeue object
 * @param size The size of the memory region
 * @param data The memory region
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_dequeue_push_first(zylib_private_dequeue_t *obj, uint64_t size, const void *data);

/**
 * Insert a node at the end of a dequeue
 * @param obj The dequeue object
 * @param size The size of the memory region
 * @param data The memory region
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_dequeue_push_last(zylib_private_dequeue_t *obj, uint64_t size, const void *data);

/**
 * Deconstruct the node at the beginning of a dequeue
 * @param obj The dequeue object
 */
ZYLIB_NONNULL
void zylib_private_dequeue_discard_first(zylib_private_dequeue_t *obj);

/**
 * Deconstruct the node at the end of a dequeue
 * @param obj The dequeue object
 */
ZYLIB_NONNULL
void zylib_private_dequeue_discard_last(zylib_private_dequeue_t *obj);

/**
 * Retrieve the node at the beginning of a dequeue
 * @param obj The dequeue object
 * @param size The pointer to the size of the memory region
 * @param data The pointer to the memory region
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_dequeue_peek_first(const zylib_private_dequeue_t *obj, uint64_t *size, const void **data);

/**
 * Retrieve the node at the end of a dequeue
 * @param obj The dequeue object
 * @param size The pointer to the size of the memory region
 * @param data The pointer to the memory region
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_dequeue_peek_last(const zylib_private_dequeue_t *obj, uint64_t *size, const void **data);

/**
 * Retrieve the number of nodes stored within a dequeue
 * @param obj The dequeue object
 * @return The number of nodes
 */
ZYLIB_NONNULL
uint64_t zylib_private_dequeue_size(const zylib_private_dequeue_t *obj);

/**
 * Retrieve whether or not there are any nodes stored within a dequeue
 * @param obj The dequeue object
 * @return True if and only if the object is empty
 */
ZYLIB_NONNULL
_Bool zylib_private_dequeue_is_empty(const zylib_private_dequeue_t *obj);

ZYLIB_END_DECLS
