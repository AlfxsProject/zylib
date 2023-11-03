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
#include "zylib_alloc.h"

/*
 * Types
 */

typedef struct zylib_error_box_s zylib_error_box_t;
typedef struct zylib_error_s zylib_error_t;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((nonnull)) zylib_return_t zylib_error_construct(zylib_error_t **err, const zylib_alloc_t *alloc);
    __attribute__((nonnull)) void zylib_error_destruct(zylib_error_t **err);
    __attribute__((nonnull)) void zylib_error_clear(zylib_error_t *err);
    __attribute__((nonnull(1, 3, 5))) zylib_return_t zylib_error_push_first(zylib_error_t *err, int64_t code,
                                                                            const char *file, size_t line,
                                                                            const char *function,
                                                                            const zylib_box_t *box);
    __attribute__((nonnull(1, 3, 5))) zylib_return_t zylib_error_push_last(zylib_error_t *err, int64_t code,
                                                                           const char *file, size_t line,
                                                                           const char *function,
                                                                           const zylib_box_t *box);
    __attribute__((nonnull)) void zylib_error_discard_first(zylib_error_t *err);
    __attribute__((nonnull)) void zylib_error_discard_last(zylib_error_t *err);
    __attribute__((nonnull)) zylib_error_box_t *zylib_error_peek_first(const zylib_error_t *err);
    __attribute__((nonnull)) zylib_error_box_t *zylib_error_peek_last(const zylib_error_t *err);
    __attribute__((nonnull)) size_t zylib_error_size(const zylib_error_t *err);
    __attribute__((nonnull)) bool zylib_error_is_empty(const zylib_error_t *err);
    __attribute__((nonnull)) int64_t zylib_error_box_peek_code(const zylib_error_box_t *bx);
    __attribute__((nonnull)) const char *zylib_error_box_peek_file(const zylib_error_box_t *bx);
    __attribute__((nonnull)) size_t zylib_error_box_peek_line_number(const zylib_error_box_t *bx);
    __attribute__((nonnull)) const char *zylib_error_box_peek_function(const zylib_error_box_t *bx);
    __attribute__((nonnull(1))) const void *zylib_error_box_peek_opaque(const zylib_error_box_t *bx, size_t *size);

#ifdef __cplusplus
}
#endif
