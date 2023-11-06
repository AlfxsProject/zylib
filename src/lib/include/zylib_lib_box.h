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

typedef struct zylib_lib_box_s zylib_lib_box_t;

ZYLIB_BEGIN_DECLS

ZYLIB_NONNULL
_Bool zylib_lib_box_construct(zylib_lib_box_t **box, const zylib_allocator_t *allocator, uint64_t size,
                              const void *data);

ZYLIB_NONNULL
void zylib_lib_box_destruct(zylib_lib_box_t **box);

ZYLIB_NONNULL
_Bool zylib_lib_box_append(zylib_lib_box_t **box, uint64_t size, const void *data);

ZYLIB_NONNULL
_Bool zylib_lib_box_split_latter(const zylib_lib_box_t *box, uint64_t index, uint64_t *size, const void **data);

ZYLIB_NONNULL
_Bool zylib_lib_box_peek_size(const zylib_lib_box_t *box, uint64_t *size);

ZYLIB_NONNULL
_Bool zylib_lib_box_peek_data(const zylib_lib_box_t *box, const void **data);

ZYLIB_END_DECLS
