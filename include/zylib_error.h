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

/*
 * Types
 */

typedef struct zylib_error_box_s zylib_error_box_t;
typedef struct zylib_error_s zylib_error_t;

/*
 * Functions
 */

ZYLIB_BEGIN_DECLS

_Bool zylib_error_construct(zylib_error_t **p_error, const zylib_allocator_t *allocator);
void zylib_error_destruct(zylib_error_t **p_error);
void zylib_error_clear(zylib_error_t *error);
_Bool zylib_error_push_first(zylib_error_t *error, int64_t code, const char *file, uint64_t line, const char *function,
                             uint64_t size, const void *p_void);
_Bool zylib_error_push_last(zylib_error_t *error, int64_t code, const char *file, uint64_t line, const char *function,
                            uint64_t size, const void *p_void);
void zylib_error_discard_first(zylib_error_t *error);
void zylib_error_discard_last(zylib_error_t *error);
const zylib_error_box_t *zylib_error_peek_first(const zylib_error_t *error);
const zylib_error_box_t *zylib_error_peek_last(const zylib_error_t *error);
uint64_t zylib_error_size(const zylib_error_t *error);
_Bool zylib_error_is_empty(const zylib_error_t *error);
int64_t zylib_error_box_peek_code(const zylib_error_box_t *error_box);
const char *zylib_error_box_peek_file(const zylib_error_box_t *error_box);
uint64_t zylib_error_box_peek_line_number(const zylib_error_box_t *error_box);
const char *zylib_error_box_peek_function(const zylib_error_box_t *error_box);
const void *zylib_error_box_peek_auxiliary_data(const zylib_error_box_t *error_box, uint64_t *p_size);

ZYLIB_END_DECLS
