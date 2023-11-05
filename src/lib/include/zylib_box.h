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

typedef struct zylib_box_s zylib_box_t;

ZYLIB_BEGIN_DECLS

_Bool zylib_box_construct(zylib_box_t **p_box, const zylib_allocator_t *allocator, uint64_t size, const void *p_void);
_Bool zylib_box_append(zylib_box_t **dest, const zylib_allocator_t *allocator, uint64_t size, const void *p_void);
const void *zylib_box_split_latter(const zylib_box_t *src, uint64_t index, uint64_t *size);
void zylib_box_destruct(zylib_box_t **p_box, const zylib_allocator_t *allocator);
uint64_t zylib_box_peek_size(const zylib_box_t *box);
const void *zylib_box_peek_data(const zylib_box_t *box);

ZYLIB_END_DECLS
