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
#include "zylib_allocator.h"
#include <stdint.h>

typedef void *zylib_dequeue_t;

ZYLIB_BEGIN_DECLS

_Bool zylib_dequeue_construct(zylib_dequeue_t **obj, const zylib_allocator_t *allocator);
void zylib_dequeue_destruct(zylib_dequeue_t **obj);
void zylib_dequeue_clear(zylib_dequeue_t *obj);
_Bool zylib_dequeue_push_first(zylib_dequeue_t *obj, uint64_t size, const void *data);
_Bool zylib_dequeue_push_last(zylib_dequeue_t *obj, uint64_t size, const void *data);
void zylib_dequeue_discard_first(zylib_dequeue_t *obj);
void zylib_dequeue_discard_last(zylib_dequeue_t *obj);
const void *zylib_dequeue_peek_first(const zylib_dequeue_t *obj, uint64_t *size);
const void *zylib_dequeue_peek_last(const zylib_dequeue_t *obj, uint64_t *size);
uint64_t zylib_dequeue_size(const zylib_dequeue_t *obj);
_Bool zylib_dequeue_is_empty(const zylib_dequeue_t *obj);

ZYLIB_END_DECLS
