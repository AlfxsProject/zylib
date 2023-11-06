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

typedef struct zylib_private_dequeue_s zylib_private_dequeue_t;

ZYLIB_BEGIN_DECLS

ZYLIB_NONNULL
_Bool zylib_private_dequeue_construct(zylib_private_dequeue_t **obj, const zylib_private_allocator_t *allocator);

ZYLIB_NONNULL
void zylib_private_dequeue_destruct(zylib_private_dequeue_t **obj);

ZYLIB_NONNULL
void zylib_private_dequeue_clear(zylib_private_dequeue_t *obj);

ZYLIB_NONNULL
_Bool zylib_private_dequeue_push_first(zylib_private_dequeue_t *obj, uint64_t size, const void *data);

ZYLIB_NONNULL
_Bool zylib_private_dequeue_push_last(zylib_private_dequeue_t *obj, uint64_t size, const void *data);

ZYLIB_NONNULL
void zylib_private_dequeue_discard_first(zylib_private_dequeue_t *obj);

ZYLIB_NONNULL
void zylib_private_dequeue_discard_last(zylib_private_dequeue_t *obj);

ZYLIB_NONNULL
_Bool zylib_private_dequeue_peek_first(const zylib_private_dequeue_t *obj, uint64_t *size, const void **data);

ZYLIB_NONNULL
_Bool zylib_private_dequeue_peek_last(const zylib_private_dequeue_t *obj, uint64_t *size, const void **data);

ZYLIB_NONNULL
uint64_t zylib_private_dequeue_size(const zylib_private_dequeue_t *obj);

ZYLIB_NONNULL
_Bool zylib_private_dequeue_is_empty(const zylib_private_dequeue_t *obj);

ZYLIB_END_DECLS
