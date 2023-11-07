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

typedef struct zylib_private_error_box_s zylib_private_error_box_t;
typedef struct zylib_private_error_s zylib_private_error_t;

ZYLIB_BEGIN_DECLS

ZYLIB_NONNULL
_Bool zylib_private_error_construct(zylib_private_error_t **obj, const zylib_private_allocator_t *allocator);

ZYLIB_NONNULL
void zylib_private_error_destruct(zylib_private_error_t **obj);

ZYLIB_NONNULL
void zylib_private_error_clear(zylib_private_error_t *obj);

ZYLIB_NONNULL
_Bool zylib_private_error_push_first(zylib_private_error_t *obj, int64_t error_code, const char *file_name,
                                     uint64_t line_number, const char *function_name, uint64_t auxiliary_size,
                                     const void *auxiliary_data);

ZYLIB_NONNULL
_Bool zylib_private_error_push_last(zylib_private_error_t *obj, int64_t error_code, const char *file_name,
                                    uint64_t line_number, const char *function_name, uint64_t auxiliary_size,
                                    const void *auxiliary_data);

ZYLIB_NONNULL
void zylib_private_error_discard_first(zylib_private_error_t *obj);

ZYLIB_NONNULL
void zylib_private_error_discard_last(zylib_private_error_t *obj);

ZYLIB_NONNULL
const zylib_private_error_box_t *zylib_private_error_peek_first(const zylib_private_error_t *obj);

ZYLIB_NONNULL
const zylib_private_error_box_t *zylib_private_error_peek_last(const zylib_private_error_t *obj);

ZYLIB_NONNULL
uint64_t zylib_private_error_size(const zylib_private_error_t *obj);

ZYLIB_NONNULL
_Bool zylib_private_error_is_empty(const zylib_private_error_t *obj);

ZYLIB_NONNULL
int64_t zylib_private_error_box_peek_error_code(const zylib_private_error_box_t *obj);

ZYLIB_NONNULL
const char *zylib_private_error_box_peek_file_name(const zylib_private_error_box_t *obj);

ZYLIB_NONNULL
uint64_t zylib_private_error_box_peek_line_number(const zylib_private_error_box_t *obj);

ZYLIB_NONNULL
const char *zylib_private_error_box_peek_function_name(const zylib_private_error_box_t *obj);

ZYLIB_NONNULL
uint64_t zylib_private_error_box_peek_auxiliary_size(const zylib_private_error_box_t *obj);

ZYLIB_NONNULL
const void *zylib_private_error_box_peek_auxiliary_data(const zylib_private_error_box_t *obj);

ZYLIB_END_DECLS
